// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyRoamingPoint.generated.h"

UCLASS()
class TOPDOWNGAME_1_API AEnemyRoamingPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyRoamingPoint();

	// œpœjˆÊ’u‚ğ—v‘f”Ô†‚Åæ“¾
	FVector GetRoamingPoint(int const index)const { return RoamingPoints[index]; }
	// œpœj”z—ñ‚Ì—v‘f”‚ğæ“¾
	int PointNum()const { return RoamingPoints.Num(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// œpœjˆÊ’u‚ğ”CˆÓ‚Åİ’è‚Å‚«‚é”z—ñ‚ğ€”õ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (MakeEditWidget = "true", AllowPrivateAccess = "true"))
	TArray<FVector>RoamingPoints;
};
