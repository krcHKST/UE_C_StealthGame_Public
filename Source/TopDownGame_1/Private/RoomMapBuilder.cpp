// RoomMapBuilder.cpp

#include "RoomMapBuilder.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Containers/Queue.h"
#include "Algo/RandomShuffle.h"
#include "NavigationSystem.h"

ARoomMapBuilder::ARoomMapBuilder()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ARoomMapBuilder::BeginPlay()
{
    Super::BeginPlay();

    if (!ensure(StartRoomClass != nullptr) ||
        !ensure(EndRoomClass != nullptr) ||
        !ensure(NormalRoomClasses.Num() > 0))
    {
        UE_LOG(LogTemp, Error, TEXT("RoomMapBuilder: Room classes are not set correctly in the editor!"));
        return;
    }

#if WITH_EDITOR
    const float gridTotalWidth = GridWidth * RoomSpacing;
    const float gridTotalHeight = GridHeight * RoomSpacing;
    const FVector center = FVector::ZeroVector;
    const FVector extent = FVector(gridTotalWidth / 2.0f, gridTotalHeight / 2.0f, 100.0f);
    DrawDebugBox(GetWorld(), center, extent, FColor::Yellow, true, -1, 0, 10.0f);
#endif

    GenerateMapWithRetry();
}

void ARoomMapBuilder::GenerateMapWithRetry()
{
    for (int32 attempt = 0; attempt < MaxGenerationAttempts; ++attempt)
    {
        if (TryMapCreationLogics())
        {
            if (PathVerification())
            {
                UE_LOG(LogTemp, Log, TEXT("Dungeon generated successfully on attempt %d."), attempt + 1);

                UpdateAllDoorStates();

#if WITH_EDITOR
                DrawDebugGrid();
#endif

                // ナビゲーションシステムを取得
                if (UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
                {
                    UE_LOG(LogTemp, Log, TEXT("Requesting NavMesh rebuild."));
                    // ナビメッシュの再構築を非同期で開始する
                    NavSystem->Build();
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to get Navigation System. NavMesh will not be rebuilt."));
                }
                return; // 成功
            }
        }

        //失敗した場合、生成したアクターを全て破棄して再試行する
#if WITH_EDITOR
        UKismetSystemLibrary::FlushPersistentDebugLines(GetWorld());
#endif
        CleanupGeneratedRooms();
    }

    UE_LOG(LogTemp, Error, TEXT("Failed to generate a valid dungeon after %d attempts. Check parameters."), MaxGenerationAttempts);
}

bool ARoomMapBuilder::TryMapCreationLogics()
{
    RoomGrid.Empty();
    StartCoord = FIntPoint(0, 0);
    PlaceRoom(StartCoord, StartRoomClass, ERoomType::Start);

    TArray<FIntPoint> openCoords = { StartCoord };
    TSet<FIntPoint> occupiedCoords; //重複防止用
    occupiedCoords.Add(StartCoord);

    int32 roomCount = 1;
    const int32 targetRoomCount = FMath::RandRange(MinRoomCount, MaxRoomCount);

    while (roomCount < targetRoomCount && !openCoords.IsEmpty())
    {
        const int32 baseIndex = FMath::RandRange(0, openCoords.Num() - 1);
        const FIntPoint baseCoord = openCoords[baseIndex];

        TArray<FIntPoint> directions = { FIntPoint(1, 0), FIntPoint(-1, 0), FIntPoint(0, 1), FIntPoint(0, -1) };
        Algo::RandomShuffle(directions);

        bool bPlacedNewRoom = false;
        for (const FIntPoint& dir : directions)
        {
            FIntPoint nextCoord = baseCoord + dir;
            if (!CanPlaceRoomAt(nextCoord)) continue;

            if (occupiedCoords.Contains(nextCoord))
            {
                continue; //すでに部屋がある座標はスキップ
            }

            TArray<TSubclassOf<ARoomBase>> compatibleRooms = FindCompatibleRooms(baseCoord, nextCoord);
            if (compatibleRooms.IsEmpty()) continue;

            TSubclassOf<ARoomBase> roomToPlace = compatibleRooms[FMath::RandRange(0, compatibleRooms.Num() - 1)];
            PlaceRoom(nextCoord, roomToPlace, ERoomType::Normal);

            openCoords.Add(nextCoord);
            occupiedCoords.Add(nextCoord); //記録
            roomCount++;
            bPlacedNewRoom = true;
            break;
        }

        if (!bPlacedNewRoom)
        {
            openCoords.RemoveAtSwap(baseIndex);
        }

        if (roomCount >= targetRoomCount)
        {
            break;
        }
    }

    if (openCoords.Num() > 1)
    {
        FIntPoint farthestCoord = StartCoord;
        float maxDistSq = -1.0f;

        for (const FIntPoint& coord : openCoords)
        {
            if (coord == StartCoord) continue;

            const float distSq = FVector2D::DistSquared(FVector2D(coord.X, coord.Y), FVector2D(StartCoord.X, StartCoord.Y));
            if (distSq > maxDistSq)
            {
                maxDistSq = distSq;
                farthestCoord = coord;
            }
        }

        //既存の部屋をEndRoomに置き換える処理
        //置き換える対象の部屋情報を取得
        FRoomInfo* existingRoomInfo = RoomGrid.Find(farthestCoord);
        if (existingRoomInfo && existingRoomInfo->Room)
        {
            //古いアクターを破棄する
            existingRoomInfo->Room->Destroy();
        }

        //RoomGridから古い情報を削除
        RoomGrid.Remove(farthestCoord);

        //EndCoordを設定し、新しいEndRoomを配置する
        EndCoord = farthestCoord;
        PlaceRoom(EndCoord, EndRoomClass, ERoomType::End);

        return true;
    }

    return false;
}

bool ARoomMapBuilder::CanPlaceRoomAt(const FIntPoint& Coord)
{
    return !RoomGrid.Contains(Coord)
        && Coord.X >= -GridWidth / 2 && Coord.X <= GridWidth / 2
        && Coord.Y >= -GridHeight / 2 && Coord.Y <= GridHeight / 2;
}

void ARoomMapBuilder::PlaceRoom(const FIntPoint& Coord, TSubclassOf<ARoomBase> RoomClass, ERoomType RoomType)
{
    if (!ensure(RoomClass != nullptr)) return;

    // 念のため RoomGrid に存在する座標ならスキップ
    if (RoomGrid.Contains(Coord))
    {
        UE_LOG(LogTemp, Warning, TEXT("Duplicate placement attempt at (%d, %d). Skipped."), Coord.X, Coord.Y);
        return;
    }

    FVector spawnLocation = FVector(Coord.X * RoomSpacing, Coord.Y * RoomSpacing, 0);
    ARoomBase* room = GetWorld()->SpawnActor<ARoomBase>(RoomClass, spawnLocation, FRotator::ZeroRotator);

    if (room)
    {
        RoomGrid.Add(Coord, { room, RoomType });

#if WITH_EDITOR
        FColor debugColor = (RoomType == ERoomType::Start) ? FColor::Blue :
            (RoomType == ERoomType::End) ? FColor::Red :
            FColor::Green;
        DrawDebugBox(GetWorld(), spawnLocation, FVector(400, 400, 100), debugColor, true, -1, 0, 5.0f);
#endif

        UE_LOG(LogTemp, Log, TEXT("Placed %s at (%d, %d)"),
            *RoomClass->GetName(), Coord.X, Coord.Y);
    }
}

bool ARoomMapBuilder::CheckRoomConnection(const FIntPoint& FromCoord, const FIntPoint& ToCoord, const ARoomBase* ToRoomCDO)
{
    const FRoomInfo* fromRoomInfo = RoomGrid.Find(FromCoord);
    if (!fromRoomInfo || !fromRoomInfo->Room || !ToRoomCDO) return false;

    const ARoomBase* fromRoomCDO = fromRoomInfo->Room->GetClass()->GetDefaultObject<ARoomBase>();
    const FIntPoint dir = ToCoord - FromCoord;

    if (dir == FIntPoint(1, 0)) return fromRoomCDO->bCanConnectForward && ToRoomCDO->bCanConnectBackward;
    if (dir == FIntPoint(-1, 0)) return fromRoomCDO->bCanConnectBackward && ToRoomCDO->bCanConnectForward;
    if (dir == FIntPoint(0, 1)) return fromRoomCDO->bCanConnectRight && ToRoomCDO->bCanConnectLeft;
    if (dir == FIntPoint(0, -1)) return fromRoomCDO->bCanConnectLeft && ToRoomCDO->bCanConnectRight;

    return false;
}

TArray<TSubclassOf<ARoomBase>> ARoomMapBuilder::FindCompatibleRooms(const FIntPoint& FromCoord, const FIntPoint& ToCoord)
{
    TArray<TSubclassOf<ARoomBase>> compatibleRooms;
    for (TSubclassOf<ARoomBase> roomClass : NormalRoomClasses)
    {
        if (roomClass)
        {
            const ARoomBase* roomCDO = roomClass->GetDefaultObject<ARoomBase>();
            if (CheckRoomConnection(FromCoord, ToCoord, roomCDO))
            {
                compatibleRooms.Add(roomClass);
            }
        }
    }
    return compatibleRooms;
}

int32 ARoomMapBuilder::GetShortestPathLength()
{
    if (!RoomGrid.Contains(StartCoord) || !RoomGrid.Contains(EndCoord)) return -1;

    TQueue<FIntPoint> queue;
    TMap<FIntPoint, int32> distanceMap;

    queue.Enqueue(StartCoord);
    distanceMap.Add(StartCoord, 0);

    FIntPoint currentCoord;
    while (queue.Dequeue(currentCoord))
    {
        if (currentCoord == EndCoord)
        {
            return distanceMap[currentCoord];
        }

        const TArray<FIntPoint> directions = { FIntPoint(1, 0), FIntPoint(-1, 0), FIntPoint(0, 1), FIntPoint(0, -1) };
        for (const FIntPoint& dir : directions)
        {
            FIntPoint nextCoord = currentCoord + dir;
            if (RoomGrid.Contains(nextCoord) && !distanceMap.Contains(nextCoord))
            {
                const ARoomBase* nextRoomCDO = RoomGrid.Find(nextCoord)->Room->GetClass()->GetDefaultObject<ARoomBase>();
                if (CheckRoomConnection(currentCoord, nextCoord, nextRoomCDO))
                {
                    distanceMap.Add(nextCoord, distanceMap[currentCoord] + 1);
                    queue.Enqueue(nextCoord);
                }
            }
        }
    }

    return -1;
}

bool ARoomMapBuilder::PathVerification()
{
    const int32 pathLength = GetShortestPathLength();
    if (pathLength >= MinPathLength)
    {
        UE_LOG(LogTemp, Log, TEXT("Path verification successful. Path length: %d (min: %d)"), pathLength, MinPathLength);
        // ここで各部屋のドアを開ける処理などを呼び出すかも
        return true;
    }

    UE_LOG(LogTemp, Warning, TEXT("Path verification failed. Path length %d is less than min %d."), pathLength, MinPathLength);
    return false;
}

// 生成に失敗した際に全てのアクターを破棄する
void ARoomMapBuilder::CleanupGeneratedRooms()
{
    for (auto& Elem : RoomGrid)
    {
        if (Elem.Value.Room)
        {
            Elem.Value.Room->Destroy();
        }
    }
    RoomGrid.Empty();
}

void ARoomMapBuilder::UpdateAllDoorStates()
{
    //RoomGridの全ての部屋
    for (const auto& Elem : RoomGrid)
    {
        const FIntPoint& currentCoord = Elem.Key;
        ARoomBase* currentRoom = Elem.Value.Room;
        if (!currentRoom) continue;

        //現在の部屋のCDOを取得
        const ARoomBase* currentRoomCDO = currentRoom->GetClass()->GetDefaultObject<ARoomBase>();

        //Forward方向
        FIntPoint neighborCoordForward(currentCoord.X + 1, currentCoord.Y);
        if (const FRoomInfo* neighborInfo = RoomGrid.Find(neighborCoordForward))
        {
            if (neighborInfo->Room)
            {
                const ARoomBase* neighborRoomCDO = neighborInfo->Room->GetClass()->GetDefaultObject<ARoomBase>();
                bool bCanConnect = currentRoomCDO->bCanConnectForward && neighborRoomCDO->bCanConnectBackward;
                currentRoom->SetDoorCanOpen(EDoorDirection::Forward, bCanConnect);
            }
        }
        else
        {
            currentRoom->SetDoorCanOpen(EDoorDirection::Forward, false);
        }

        //Backward方向
        FIntPoint neighborCoordBackward(currentCoord.X - 1, currentCoord.Y);
        if (const FRoomInfo* neighborInfo = RoomGrid.Find(neighborCoordBackward))
        {
            if (neighborInfo->Room)
            {
                const ARoomBase* neighborRoomCDO = neighborInfo->Room->GetClass()->GetDefaultObject<ARoomBase>();
                bool bCanConnect = currentRoomCDO->bCanConnectBackward && neighborRoomCDO->bCanConnectForward;
                currentRoom->SetDoorCanOpen(EDoorDirection::Backward, bCanConnect);
            }
        }
        else
        {
            currentRoom->SetDoorCanOpen(EDoorDirection::Backward, false);
        }

        //Right方向
        FIntPoint neighborCoordRight(currentCoord.X, currentCoord.Y + 1);
        if (const FRoomInfo* neighborInfo = RoomGrid.Find(neighborCoordRight))
        {
            if (neighborInfo->Room)
            {
                const ARoomBase* neighborRoomCDO = neighborInfo->Room->GetClass()->GetDefaultObject<ARoomBase>();
                bool bCanConnect = currentRoomCDO->bCanConnectRight && neighborRoomCDO->bCanConnectLeft;
                currentRoom->SetDoorCanOpen(EDoorDirection::Right, bCanConnect);
            }
        }
        else
        {
            currentRoom->SetDoorCanOpen(EDoorDirection::Right, false);
        }

        //Left方向
        FIntPoint neighborCoordLeft(currentCoord.X, currentCoord.Y - 1);
        if (const FRoomInfo* neighborInfo = RoomGrid.Find(neighborCoordLeft))
        {
            if (neighborInfo->Room)
            {
                const ARoomBase* neighborRoomCDO = neighborInfo->Room->GetClass()->GetDefaultObject<ARoomBase>();
                bool bCanConnect = currentRoomCDO->bCanConnectLeft && neighborRoomCDO->bCanConnectRight;
                currentRoom->SetDoorCanOpen(EDoorDirection::Left, bCanConnect);
            }
        }
        else
        {
            currentRoom->SetDoorCanOpen(EDoorDirection::Left, false);
        }
    }
}

void ARoomMapBuilder::DrawDebugGrid()
{
    UWorld* World = GetWorld();
    if (!World) return;

    //RoomGrid内の全ての部屋をループ
    for (const auto& Elem : RoomGrid)
    {
        const FIntPoint& Coord = Elem.Key;
        const FRoomInfo& Info = Elem.Value;
        ARoomBase* Room = Info.Room;

        if (!Room) continue;

        FVector RoomCenter = Room->GetActorLocation();

        //グリッド座標と部屋タイプをテキストで描画
        FString RoomTypeStr = UEnum::GetValueAsString(Info.RoomType);
        RoomTypeStr.RemoveFromStart(TEXT("ERoomType::"));
        FString DebugText = FString::Printf(TEXT("Coord: (%d, %d)\nType: %s"), Coord.X, Coord.Y, *RoomTypeStr);
        //少し高い位置に白い文字で永続的に表示
        DrawDebugString(World, RoomCenter + FVector(0, 0, 200.f), DebugText, nullptr, FColor::White, -1.0f, true);

        //接続可能な方向を緑色の線で描画
        const float LineLength = RoomSpacing * 0.4f;
        const float LineThickness = 10.0f;

        //Forward
        if (Room->bCanConnectForward)
        {
            DrawDebugLine(World, RoomCenter, RoomCenter + FVector(LineLength, 0, 0), FColor::Green, true, -1, 0, LineThickness);
        }
        //Backward
        if (Room->bCanConnectBackward)
        {
            DrawDebugLine(World, RoomCenter, RoomCenter - FVector(LineLength, 0, 0), FColor::Green, true, -1, 0, LineThickness);
        }
        //Right
        if (Room->bCanConnectRight)
        {
            DrawDebugLine(World, RoomCenter, RoomCenter + FVector(0, LineLength, 0), FColor::Green, true, -1, 0, LineThickness);
        }
        //Left
        if (Room->bCanConnectLeft)
        {
            DrawDebugLine(World, RoomCenter, RoomCenter - FVector(0, LineLength, 0), FColor::Green, true, -1, 0, LineThickness);
        }
    }
}