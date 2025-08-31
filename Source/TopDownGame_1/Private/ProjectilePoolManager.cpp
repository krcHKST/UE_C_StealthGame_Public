// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilePoolManager.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileBase.h"

void AProjectilePoolManager::BeginPlay()
{
    Super::BeginPlay();

    for (auto& elem : InitialPoolSizes) 
    {
        TSubclassOf<AProjectileBase> projectileClass = elem.Key; 
        int32 poolSize = elem.Value; 

        TArray<AProjectileBase*>& pool = ProjectilePools.FindOrAdd(projectileClass); 

        for (int32 i = 0; i < poolSize; ++i) //初期生成→待機状態
        {
            AProjectileBase* projectile = GenerateProjectile(projectileClass,pool);
            if(projectile)projectile->DeactivateProjectile();
        }
    }
}

AProjectileBase* AProjectilePoolManager::GenerateProjectile(TSubclassOf<AProjectileBase> projectileClass, TArray<AProjectileBase*>& pool)
{
    AProjectileBase* newProjectile = GetWorld()->SpawnActor<AProjectileBase>(projectileClass, FVector::ZeroVector, FRotator::ZeroRotator);
    if (newProjectile)
    {
        newProjectile->PoolManager = this;
        pool.Add(newProjectile);
        return newProjectile;
    }

    return nullptr;
}

AProjectileBase* AProjectilePoolManager::GetProjectileFromPool(TSubclassOf<AProjectileBase> projectileClass)
{
    if (!projectileClass) return nullptr;
    TArray<AProjectileBase*>& pool = ProjectilePools.FindOrAdd(projectileClass);
    AProjectileBase* projectileToUse = nullptr;
    //利用可能な弾を探す
    for (AProjectileBase* projectile : pool)
    {
        if (projectile && !projectile->IsActorTickEnabled())
        {
            projectileToUse = projectile;
            break;
        }
    }

    //見つからなかったら新規生成
    if (!projectileToUse)
    {
        projectileToUse = GenerateProjectile(projectileClass, pool);
    }
    if (projectileToUse)
    {
        //アクティブ化
        projectileToUse->SetActorEnableCollision(true);
        projectileToUse->SetActorHiddenInGame(false);
        projectileToUse->SetActorTickEnabled(true);
        
        return projectileToUse;
    }

    //生成にも失敗した場合
    return nullptr;
}

void AProjectilePoolManager::ReturnProjectileToPool(AProjectileBase* projectile) 
{
    if (projectile) 
    {
        projectile->DeactivateProjectile(); 
    }
}