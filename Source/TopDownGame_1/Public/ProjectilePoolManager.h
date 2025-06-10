// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectilePoolManager.generated.h"

class AProjectileBase;

UCLASS()
class TOPDOWNGAME_1_API AProjectilePoolManager : public AActor
{
    GENERATED_BODY()

    AProjectileBase* GenerateProjectile(TSubclassOf<AProjectileBase> projectileClass, TArray<AProjectileBase*>& pool);
public:
    UPROPERTY(EditAnywhere, Category = "ProjectilePool")
    TMap<TSubclassOf<AProjectileBase>, int32> InitialPoolSizes;
    TMap<TSubclassOf<AProjectileBase>, TArray<AProjectileBase*>> ProjectilePools;

    virtual void BeginPlay() override;

    AProjectileBase* GetProjectileFromPool(TSubclassOf<AProjectileBase> projectileClass);
    void ReturnProjectileToPool(AProjectileBase* projectile);
};