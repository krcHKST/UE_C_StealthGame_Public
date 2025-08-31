// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomBase.h"
#include "RoomMapBuilder.generated.h"

/// @brief 部屋ごとの情報纏める用
USTRUCT()
struct FRoomInfo
{
    GENERATED_BODY()
    UPROPERTY()
    class ARoomBase* Room = nullptr;
    UPROPERTY()
    ERoomType RoomType = ERoomType::Normal;
};

UCLASS()
class TOPDOWNGAME_1_API ARoomMapBuilder : public AActor
{
    GENERATED_BODY()
public:
    ARoomMapBuilder();
protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Dungeon Settings|Room Classes")
    TSubclassOf<class ARoomBase> StartRoomClass;

    UPROPERTY(EditAnywhere, Category = "Dungeon Settings|Room Classes")
    TSubclassOf<class ARoomBase> EndRoomClass;

    UPROPERTY(EditAnywhere, Category = "Dungeon Settings|Room Classes")
    TArray<TSubclassOf<class ARoomBase>> NormalRoomClasses;

    UPROPERTY(EditAnywhere, Category = "Dungeon Settings|Grid")
    int32 GridWidth = 5;

    UPROPERTY(EditAnywhere, Category = "Dungeon Settings|Grid")
    int32 GridHeight = 5;

    UPROPERTY(EditAnywhere, Category = "Dungeon Settings|Room Count")
    int32 MaxRoomCount = 12;

    UPROPERTY(EditAnywhere, Category = "Dungeon Settings|Room Count")
    int32 MinRoomCount = 6;

    UPROPERTY(EditAnywhere, Category = "Dungeon Settings|Grid")
    float RoomSpacing = 1000.0f;

    /// @brief StartからEndまでの最短経路の最小長
    UPROPERTY(EditAnywhere, Category = "Dungeon Settings|Validation")
    int32 MinPathLength = 5;

    /// @brief マップ生成の最大試行回数 (無限ループ防止)
    UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings|Validation")
    int32 MaxGenerationAttempts = 50;

private:
    TMap<FIntPoint, FRoomInfo> RoomGrid;
    FIntPoint StartCoord;
    FIntPoint EndCoord;

    //生成プロセス関数
    void GenerateMapWithRetry();
    bool TryMapCreationLogics();
    bool PathVerification();
    void CleanupGeneratedRooms();
    void UpdateAllDoorStates();
    void DrawDebugGrid();

    bool CanPlaceRoomAt(const FIntPoint& Coord);
    void PlaceRoom(const FIntPoint& Coord, TSubclassOf<class ARoomBase> RoomClass, ERoomType RoomType);
    TArray<TSubclassOf<class ARoomBase>> FindCompatibleRooms(const FIntPoint& FromCoord, const FIntPoint& ToCoord);
    bool CheckRoomConnection(const FIntPoint& FromCoord, const FIntPoint& ToCoord, const class ARoomBase* ToRoomCDO);
    int32 GetShortestPathLength();
};