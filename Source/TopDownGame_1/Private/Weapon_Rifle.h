// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Weapon_Rifle.generated.h"

/**
 * 
 */
UCLASS()
class AWeapon_Rifle : public AWeaponBase
{
	GENERATED_BODY()

	void BeginPlay() override;

	void SpawnProjectile() override;
public:
	void Fire() override;
	void EndFire() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;
	UPROPERTY(EditAnywhere)
	FRotator ShotDirection;
	UPROPERTY(EditDefaultsOnly)
	FName SocketName;
	UPROPERTY(EditDefaultsOnly)
	float DamageAmount;
};
