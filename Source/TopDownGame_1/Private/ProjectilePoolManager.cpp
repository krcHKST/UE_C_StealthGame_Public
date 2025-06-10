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

        for (int32 i = 0; i < poolSize; ++i) //‰Šú¶¬¨‘Ò‹@ó‘Ô
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

    for (AProjectileBase* projectile : pool) 
    {
        if (!projectile->IsActorTickEnabled()) 
        {
            projectile->SetActorEnableCollision(true); 
            projectile->SetActorHiddenInGame(false); 
            projectile->SetActorTickEnabled(true); 
            return projectile; 
        }
    }

    // ‹ó‚«‚ª‚È‚¯‚ê‚ÎV‹K¶¬
    return GenerateProjectile(projectileClass,pool);
}

void AProjectilePoolManager::ReturnProjectileToPool(AProjectileBase* projectile) 
{
    if (projectile) 
    {
        projectile->DeactivateProjectile(); 
    }
}