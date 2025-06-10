// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class ACharacterBase;
class AProjectilePoolManager;

UCLASS()
class AWeaponBase : public AActor
{
    GENERATED_BODY()

public:
    AWeaponBase();
    UPROPERTY(EditDefaultsOnly, Category = Projectile)

    TSubclassOf<class AProjectileBase> ProjectileClass;
protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float DeltaTime) override;
    UFUNCTION()
    virtual void SpawnProjectile();
    UFUNCTION()
    virtual void HandlePickUp(ACharacterBase* character);
    UPROPERTY()
    AActor* OwnerActor;
    UPROPERTY()
    AController* ProjectileInstigator;
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Root;
    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* SkeletalMesh;
    UPROPERTY(EditAnywhere)
    UParticleSystem* MuzzleFlash;
    UPROPERTY(EditAnywhere)
    class UPickUpComponent* PickUpComponent;
    UPROPERTY(EditAnywhere)
    float FireInterval;
    UPROPERTY()
    float FireIntervalTimer;
    UPROPERTY()
    bool isFiring = false;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
    FName OfficialName;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
    UTexture2D* Icon;
    UPROPERTY()
    AProjectilePoolManager* ProjectilePoolManager;
public:
    virtual void Fire();
    virtual void EndFire();
    void SetFireFlag(bool newFlag);
    virtual void Drop();

    //get,set
    UFUNCTION(BlueprintCallable)
    UPickUpComponent* GetPickUpComponent() const { return PickUpComponent; }
};