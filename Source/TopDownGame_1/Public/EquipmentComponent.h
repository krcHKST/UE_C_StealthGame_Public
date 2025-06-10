// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

class ACharacterBase;
class UWeaponComponent;
class AWeaponBase;
class ATopDownGame_1Character;
DECLARE_DELEGATE(FEquipDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOPDOWNGAME_1_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<AWeaponBase> EquipWeapon;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
	void Equip(AWeaponBase* weapon, USkeletalMeshComponent* attachMesh);
	UFUNCTION()
	void SwitchWeapon(int num);
	UFUNCTION()
	void Fire();
	UFUNCTION()
	void EndFire();
	UFUNCTION()
	void Drop();
	UFUNCTION()
	void DiscardWeapon();
	FEquipDelegate EquipEvent;

	//get,set
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* GetEquipWeapon() const { return EquipWeapon.Get(); }
};
