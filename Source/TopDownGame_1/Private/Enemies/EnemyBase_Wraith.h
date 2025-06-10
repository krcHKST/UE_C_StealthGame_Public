// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownGame_1/EnemyBaseCharacter.h"
#include "EnemyBase_Wraith.generated.h"

class AWeaponBase;
/**
 * 
 */
UCLASS()
class AEnemyBase_Wraith : public AEnemyBaseCharacter
{
	GENERATED_BODY()
private:
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void DeathReaction() override;
	AEnemyBase_Wraith();
};
