// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class AWeaponBase;
class UHealthComponent;
UCLASS()
class TOPDOWNGAME_1_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	UPROPERTY()
	AWeaponBase* HoldWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UEquipmentComponent* EquipmentComponent;
	UPROPERTY(EditDefaultsOnly)
	UHealthComponent* HealthComponent;
	UPROPERTY(EditAnywhere)
	FName EquipSocketName;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
public:	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void Fire();
	UFUNCTION()
	virtual void EndFire();
	UFUNCTION()
	virtual void DeathReaction();
	UFUNCTION()
	virtual void HandleOnDestroyed(AActor* DestroyedActor);
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeaponBase> DefaultHoldWeapon;
	//get,set
	UFUNCTION(BlueprintCallable)
	FName GetEquipSocket() const{ return EquipSocketName; }
};
