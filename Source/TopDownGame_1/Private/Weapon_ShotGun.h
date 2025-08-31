// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Weapon_ShotGun.generated.h"


UCLASS()
class AWeapon_ShotGun : public AWeaponBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SpawnProjectile() override;

public:
	virtual void Fire() override;
	virtual void EndFire() override;

private:
	/** 散弾のばらける角度（度数） */
	UPROPERTY(EditDefaultsOnly, Category = "ShotGun")
	float SpreadAngle = 5.0f;
};
