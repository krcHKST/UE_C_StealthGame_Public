#include "Weapon_ShotGun.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileBase.h"
#include "ProjectilePoolManager.h"

void AWeapon_ShotGun::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon_ShotGun::Fire()
{
	Super::Fire();
}

void AWeapon_ShotGun::EndFire()
{
	Super::EndFire();
}

void AWeapon_ShotGun::SpawnProjectile()
{
    if (!ProjectilePoolManager || !ProjectileClass || !GetOwner()) return;
    //発射処理に問題ないとき
    FireIntervalTimer = 0;

    if (CurrentAmmo < SourceStats.OneShotNum)
    {
        RequestReload();//自動リロード
        return;
    }

    CurrentAmmo -= SourceStats.OneShotNum;
    if (CurrentAmmo <= 0) CurrentAmmo = 0;

    const FTransform spawnTransform = GetProjectileSpawnTransform();
    const FVector spawnLocation = spawnTransform.GetLocation();
    const FRotator spawnRotation = spawnTransform.GetRotation().Rotator();

    //複数発射
    for (int32 i = 0; i < SourceStats.OneShotNum; i++)
    {
        const float randomPitch = FMath::FRandRange(-SpreadAngle, SpreadAngle);
        const float randomYaw = FMath::FRandRange(-SpreadAngle, SpreadAngle);
        FRotator pelletRotation = spawnRotation + FRotator(randomPitch, randomYaw, 0.f);

        AProjectileBase* projectile = InitializeProjectile();
        if (projectile)
        {
            projectile->ActivateProjectile(spawnLocation, pelletRotation);
        }
    }

    //エフェクト再生
    PlayMuzzleEffect();
}
