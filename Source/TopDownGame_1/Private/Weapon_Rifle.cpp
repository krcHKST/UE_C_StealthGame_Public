// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Rifle.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "TopDownGame_1/Public/ProjectileBase.h"
#include "ProjectilePoolManager.h"

void AWeapon_Rifle::BeginPlay()
{
	Super::BeginPlay();

}
void AWeapon_Rifle::Fire()
{
	Super::Fire();
}

void AWeapon_Rifle::EndFire()
{
	Super::EndFire();
	SpawnProjectile();
}

void AWeapon_Rifle::SpawnProjectile()
{
    if (!ProjectilePoolManager || !GetOwner()) return;

    UWorld* const World = GetWorld();
    if (World == nullptr) return;

    // エフェクト出す
    UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, SkeletalMesh, SocketName);

    const FRotator SpawnRotation = SkeletalMesh->GetSocketRotation(SocketName) + ShotDirection;
    const FVector SpawnLocation = SkeletalMesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));

    // プールから Projectile を取得
    AProjectileBase* projectile = ProjectilePoolManager->GetProjectileFromPool(ProjectileClass);
    if (projectile == nullptr)
    {
        UKismetSystemLibrary::PrintString(this, "Projectile cannot Spawn (Pool empty)", true, true, FColor::Cyan, 2.f, TEXT("None"));
        return;
    }

    // プールから取得したProjectileを配置・初期化
    //projectile->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
    projectile->SetDamageAmount(DamageAmount);
    projectile->SetOwner(this);
    projectile->SetParentActor(GetOwner());

    if (ProjectileInstigator == nullptr) {
        UKismetSystemLibrary::PrintString(this, "Projectile NotFound Instigator", true, true, FColor::Cyan, 2.f, TEXT("None"));
        return;
    }
       
    projectile->SetDamageInstigator(ProjectileInstigator);

    // Activate projectile（プール用の有効化関数）
    projectile->ActivateProjectile(SpawnLocation, SpawnRotation);
}