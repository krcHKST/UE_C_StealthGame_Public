// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
//#include "TopDownGame_1/Public/WeaponComponent.h"
#include "TopDownGame_1Character.generated.h"

UCLASS(Blueprintable)
class ATopDownGame_1Character : public ACharacterBase
{
	GENERATED_BODY()

public:
	ATopDownGame_1Character();

	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void HandleOnDestroyed(AActor* DestroyedActor) override;
	virtual void DeathReaction() override;
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/*
	getter,setter
	*/
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetMaxStamina() { return MaxStamina; }
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetCurrentStamina() { return CurrentStamina; }

	USkeletalMeshComponent* GetSkeletalMesh() const { return SkeletalMesh; }
	bool GetHasWeapon() { return bHasWeapon; }
	void SetHasWeapon(bool NewHasWeapon);
protected:
	UFUNCTION()
	void ReloadLevel();
	UPROPERTY()
	class APlayerController* PlayerController;
	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* SkeletalMesh;

	class UHealthComponent* HealthComp;
private:
	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxLife;
	UPROPERTY(EditAnywhere, Category = "Status")
	float CurrentLife;
	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxStamina;
	UPROPERTY(EditAnywhere, Category = "Status")
	float CurrentStamina;
	bool bHasWeapon;
	//UPROPERTY(EditAnywhere, Category = "Weapon")
	//UWeaponComponent*  EquipWeaponComponent;
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};

