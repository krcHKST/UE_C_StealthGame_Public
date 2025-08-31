// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPointComponent.h"
#include "RoomBase.h"
#include "CharacterBase.h"
#include "TopDownGame_1/EnemyBaseCharacter.h"

// Sets default values for this component's properties
USpawnPointComponent::USpawnPointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawnPointComponent::BeginPlay()
{
	Super::BeginPlay();

    if (!SpawnActor) return;

    UWorld* World = GetWorld();
    if (!World) return;

    AActor* ownerActor = GetOwner();
    if (!ownerActor) return;

    FActorSpawnParameters params;
    params.Owner = ownerActor;
    params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    // ワールドに Spawn
    AActor* spawnedActor = World->SpawnActor<AActor>(SpawnActor, GetComponentTransform(), params);
    if (!spawnedActor) return;
        // 親が Room かチェック
    if (ARoomBase* room = Cast<ARoomBase>(ownerActor))
    {
        // AEnemyBaseCharacter なら ParentRoom をセット
        if (auto* enemy = Cast<AEnemyBaseCharacter>(spawnedActor))
        {
            enemy->SetParentRoom(room);
            enemy->SetRoamingID(RoamingID);
        }
        // 部屋にアタッチ(必要でなければ消す）
        spawnedActor->AttachToActor(room, FAttachmentTransformRules::KeepWorldTransform);
    }
}

void USpawnPointComponent::OnRegister()
{
	Super::OnRegister();

	if (AActor* parent = GetAttachParentActor())
	{
		if (ARoomBase* room = Cast<ARoomBase>(parent))
		{
			room->RegisterSpawnPoint(this);
		}
	}
}


// Called every frame
void USpawnPointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

