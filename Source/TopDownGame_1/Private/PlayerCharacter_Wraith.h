// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownGame_1/TopDownGame_1Character.h"
#include "PlayerCharacter_Wraith.generated.h"

class AWeaponBase;
/**
 * 
 */
UCLASS()
class APlayerCharacter_Wraith : public ATopDownGame_1Character
{
	GENERATED_BODY()

private:
	UPROPERTY()
	float MouseAngle;
	UPROPERTY()
	FVector CachedDestination;
public:
	APlayerCharacter_Wraith();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void Fire() override;
	void EndFire() override;
	UPROPERTY(EditAnywhere)
	float RotationRate;
	/*UPROPERTY(EditAnywhere)
	TSubclassOf<AWeaponBase> DefaultHoldWeapon;*/
	UFUNCTION(BlueprintCallable)
	float GetMouseAngle() { return MouseAngle; };
};
