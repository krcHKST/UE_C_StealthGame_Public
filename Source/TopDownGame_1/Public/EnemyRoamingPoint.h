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

	// �p�j�ʒu��v�f�ԍ��Ŏ擾
	FVector GetRoamingPoint(int const index)const { return RoamingPoints[index]; }
	// �p�j�z��̗v�f�����擾
	int PointNum()const { return RoamingPoints.Num(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �p�j�ʒu��C�ӂŐݒ�ł���z�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (MakeEditWidget = "true", AllowPrivateAccess = "true"))
	TArray<FVector>RoamingPoints;
};
