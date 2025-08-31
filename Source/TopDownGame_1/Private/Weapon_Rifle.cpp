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
}

void AWeapon_Rifle::SpawnProjectile()
{
    Super::SpawnProjectile();
    //// エフェクト出す
    //UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, SkeletalMesh, SocketName);
    //const FRotator SpawnRotation = SkeletalMesh->GetSocketRotation(SocketName) + ShotDirection;
    //const FVector SpawnLocation = SkeletalMesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));
    //AProjectileBase* projectile = InitializeProjectile();
    //if(projectile)
    //{
    //    // Activate projectile（プール用の有効化関数）
    //    projectile->ActivateProjectile(SpawnLocation, SpawnRotation);
    //}
}