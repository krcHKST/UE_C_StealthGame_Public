// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRoamingPointComponent.h"
#include "RoomBase.h"
// Sets default values for this component's properties
UEnemyRoamingPointComponent::UEnemyRoamingPointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyRoamingPointComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UEnemyRoamingPointComponent::OnRegister()
{
	Super::OnRegister();

	if (AActor* parent = GetAttachParentActor())
	{
		if (ARoomBase* room = Cast<ARoomBase>(parent))
		{
			room->RegisterRoamingPoint(this);
		}
	}
}


// Called every frame
void UEnemyRoamingPointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UEnemyRoamingPointComponent::GetRoamingID()
{
	return RoamingID;
}

