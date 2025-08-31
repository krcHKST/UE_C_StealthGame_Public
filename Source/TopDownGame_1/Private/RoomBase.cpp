// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomBase.h"
#include "AutomaticDoor.h"
#include "Components/ChildActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyRoamingPoint.h"
#include "SpawnPointComponent.h"

// Sets default values
ARoomBase::ARoomBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	//四方向のドア
	DoorForwardComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("DoorForwardComponent"));
	DoorForwardComponent->SetupAttachment(RootComponent);
	DoorForwardComponent->SetChildActorClass(AAutomaticDoor::StaticClass());

	DoorBackwardComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("DoorBackwardComponent"));
	DoorBackwardComponent->SetupAttachment(RootComponent);
	DoorBackwardComponent->SetChildActorClass(AAutomaticDoor::StaticClass());

	DoorRightComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("DoorRightComponent"));
	DoorRightComponent->SetupAttachment(RootComponent);
	DoorRightComponent->SetChildActorClass(AAutomaticDoor::StaticClass());

	DoorLeftComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("DoorLeftComponent"));
	DoorLeftComponent->SetupAttachment(RootComponent);
	DoorLeftComponent->SetChildActorClass(AAutomaticDoor::StaticClass());
}

// Called when the game starts or when spawned
void ARoomBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoomBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors, true); // trueにすると、子の子まで再帰的にすべて取得します

	// 取得したすべての子アクターをループして破棄します
	for (AActor* ChildActor : AttachedActors)
	{
		if (IsValid(ChildActor))
		{
			ChildActor->Destroy();
		}
	}
}

AAutomaticDoor* ARoomBase::GetDoorByDirection(EDoorDirection Direction)
{
	UChildActorComponent* DoorComponent = nullptr;
	switch (Direction)
	{
	case EDoorDirection::Forward:
		DoorComponent = DoorForwardComponent;
		break;
	case EDoorDirection::Backward:
		DoorComponent = DoorBackwardComponent;
		break;
	case EDoorDirection::Right:
		DoorComponent = DoorRightComponent;
		break;
	case EDoorDirection::Left:
		DoorComponent = DoorLeftComponent;
		break;
	}

	if (!DoorComponent) return nullptr;

	if (DoorComponent->GetChildActor())
	{
		return Cast<AAutomaticDoor>(DoorComponent->GetChildActor());
	}

	return nullptr;
}

// Called every frame
void ARoomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ARoomBase::RegisterRoamingPoint(UEnemyRoamingPointComponent* registerPoint)
{
	if (!registerPoint)
	{
		return false; // 無効な参照
	}
	RoamingPoints.AddUnique(registerPoint); // 重複防止
	return true;
}

bool ARoomBase::RegisterSpawnPoint(USpawnPointComponent* registerPoint)
{
	if (!registerPoint)
	{
		return false; // 無効な参照
	}

	SpawnPoints.AddUnique(registerPoint); // 重複防止
	return true;
}

void ARoomBase::SetDoorCanOpen(EDoorDirection Direction, bool bInCanOpen)
{
	if (AAutomaticDoor* Door = GetDoorByDirection(Direction))
	{
		Door->bCanOpen = bInCanOpen;
	}
}
