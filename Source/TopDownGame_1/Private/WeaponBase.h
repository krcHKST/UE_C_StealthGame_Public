// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataAssets/WeaponStats.h"
#include "Enum/Weapon/EWeaponState.h"
#include "WeaponBase.generated.h"

class ACharacterBase;
class AProjectilePoolManager;
class AProjectileBase;
class UWeaponDataAsset;
class UWeaponStateMachineComponent;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDropped);
UCLASS()
class AWeaponBase : public AActor
{
    GENERATED_BODY()

public:
    AWeaponBase();

    UFUNCTION(BlueprintCallable)
    ///基底ではnullチェック、弾数の減少までを行い、派生で発射の挙動を定義
    virtual void SpawnProjectile();
    UFUNCTION(BlueprintCallable)
    virtual AProjectileBase* InitializeProjectile();
    UFUNCTION(BlueprintCallable)
    virtual void FullAmmo();


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State Machine")
    UWeaponStateMachineComponent* WeaponStateMachine;

    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AProjectileBase> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category = "WeaponData")
    UWeaponDataAsset* WeaponDataAsset;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundBase* FireSound;
    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundBase* ReloadStartSound;
    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundBase* ReloadEndSound;
protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    //virtual void Tick(float DeltaTime) override;
    UFUNCTION()
    virtual void HandlePickUp(ACharacterBase* character);//拾われ時イベント登録用
    UFUNCTION()
    virtual FTransform GetProjectileSpawnTransform() const;
    UFUNCTION()
    void PlayMuzzleEffect();
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
    UPROPERTY()
    class UDropItemMover* DropItemMover;
    UPROPERTY(EditAnywhere)
    float FireInterval;//発射間隔秒数
    UPROPERTY()
    float FireIntervalTimer;//発射間隔の時間計測
    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    int32 CurrentAmmo;
    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    FWeaponStats SourceStats;//元になったステータスデータ
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
    FName OfficialName;//表示名
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
    UTexture2D* Icon;//表示画像

    UPROPERTY()
    AProjectilePoolManager* ProjectilePoolManager;//オブジェクトプール用
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
    FVector MuzzleOffset;//メッシュ外観に合わせるため
    UPROPERTY(EditDefaultsOnly)
    FRotator ShotDirection;
    UPROPERTY(EditDefaultsOnly)
    FName SocketName;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float DamageAmount;
    /*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    EWeaponState WeaponState = EWeaponState::Idle;*///ステート型にしたので後で消す
    
public:
    virtual void Fire();
    virtual void EndFire();
    UFUNCTION()
    virtual void RequestReload();
    //void SetFireFlag(bool newFlag);
    UFUNCTION()
    virtual void UpdateFireTimer(float DeltaTime);
    virtual void Drop();
    UPROPERTY(BlueprintAssignable)
    FOnDropped OnDropped;
    //get,set
    UFUNCTION(BlueprintPure)
    UPickUpComponent* GetPickUpComponent() const { return PickUpComponent; }
    UFUNCTION(BlueprintPure)
    int32 GetCurrentAmmo() const { return CurrentAmmo; }
    UFUNCTION(BlueprintPure)
    bool HasAmmo() const { return CurrentAmmo > 0; }
    bool ReadyToFire() const;
    UFUNCTION(BlueprintPure)
    const FWeaponStats& GetWeaponStats() const { return SourceStats; }
};