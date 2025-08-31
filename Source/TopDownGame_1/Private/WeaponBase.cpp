// Fill out your copyright notice in the Description page of Project Settings.
#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "PickUpComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileBase.h"
#include "ProjectilePoolManager.h"
#include "EquipmentComponent.h"
#include "DataAssets/WeaponDataAsset.h"
#include "DropItemMover.h"
#include "StateMachine/Weapon/WeaponStateMachineComponent.h"
#include "StateMachine/Weapon/WeaponState_Idle.h"
#include "StateMachine/Weapon/WeaponState_Reload.h"
#include "StateMachine/Weapon/WeaponState_Fire.h"


AWeaponBase::AWeaponBase()
{
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    SkeletalMesh->SetupAttachment(RootComponent);

    PickUpComponent = CreateDefaultSubobject<UPickUpComponent>(TEXT("PickUpComp"));
    PickUpComponent->SetupAttachment(RootComponent);

    DropItemMover = CreateDefaultSubobject<UDropItemMover>(TEXT("DropItemMover"));
    //DropItemMover->RegisterComponent();

    // ステートマシンコンポーネントを生成
    WeaponStateMachine = CreateDefaultSubobject<UWeaponStateMachineComponent>(TEXT("WeaponStateMachine"));
}

void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();

    if (PickUpComponent)
    {
        PickUpComponent->OnPickUp.AddDynamic(this, &AWeaponBase::HandlePickUp);
    }

    if (WeaponDataAsset)
    {
        SourceStats = WeaponDataAsset->WeaponStats;
        CurrentAmmo = SourceStats.MagazineSize;

        FireInterval = SourceStats.FireRate;
        FireIntervalTimer = FireInterval;//一発目はすぐ出るように
        OfficialName = FName(SourceStats.WeaponName.ToString());
        Icon = WeaponDataAsset->WeaponIcon;
    }

    // ProjectilePoolManager取得
    if (!ProjectilePoolManager)
    {
        //セットされてなければ
        AActor* const foundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AProjectilePoolManager::StaticClass());
        if (foundActor) {
            AProjectilePoolManager* const poolManager = Cast<AProjectilePoolManager>(foundActor);
            if (poolManager)ProjectilePoolManager = poolManager;
            else UKismetSystemLibrary::PrintString(this, "Pool empty", true, true, FColor::Cyan, 2.f, TEXT("None"));
        }
    }
}

void AWeaponBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

//void AWeaponBase::Tick(float DeltaTime)
//{
//    Super::Tick(DeltaTime);
//
//    switch (WeaponState)
//    {
//    case EWeaponState::Idle:
//        FireIntervalTimer += DeltaTime;
//        if (FireIntervalTimer >= FireInterval)
//        {
//            FireIntervalTimer = FireInterval;
//        }
//        break;
//    case EWeaponState::Fire:
//        FireIntervalTimer += DeltaTime;
//        if (FireIntervalTimer >= FireInterval)
//        {
//            SpawnProjectile();
//        }
//        break;
//    case EWeaponState::Reload:
//        Reload(DeltaTime);
//        break;
//    }
//}

void AWeaponBase::Fire()
{
    if (WeaponStateMachine)
    {
        WeaponStateMachine->Fire();
    }
}

void AWeaponBase::EndFire()
{
    if (WeaponStateMachine)
    {
        WeaponStateMachine->EndFire();
    }
}

void AWeaponBase::RequestReload()
{
    if (WeaponStateMachine)
    {
        WeaponStateMachine->Reload();
    }
}

bool AWeaponBase::ReadyToFire() const
{
    if (FireIntervalTimer >= FireInterval) return true;
    else return false;
}


void AWeaponBase::SpawnProjectile()
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

    const FTransform SpawnTransform = GetProjectileSpawnTransform();

    AProjectileBase* projectile = InitializeProjectile();
    if (projectile)
    {
        projectile->ActivateProjectile(SpawnTransform.GetLocation(),SpawnTransform.GetRotation().Rotator());
    }

    // エフェクト再生
    PlayMuzzleEffect();
}

AProjectileBase* AWeaponBase::InitializeProjectile()
{
    // プールから Projectile を取得
    AProjectileBase* projectile = ProjectilePoolManager->GetProjectileFromPool(ProjectileClass);
    if (projectile == nullptr)
    {
        UKismetSystemLibrary::PrintString(this, "Projectile cannot Spawn (Pool empty)", true, true, FColor::Cyan, 2.f, TEXT("None"));
        return nullptr;
    }
    // プールから取得したProjectileを配置・初期化
    projectile->SetDamageAmount(SourceStats.AttackPower);
    projectile->SetOwner(this);
    projectile->SetParentActor(GetOwner());
    if (ProjectileInstigator == nullptr) {
        UKismetSystemLibrary::PrintString(this, "Projectile NotFound Instigator", true, true, FColor::Cyan, 2.f, TEXT("None"));
        return nullptr;
    }
    projectile->SetDamageInstigator(ProjectileInstigator);
    return projectile;
}

void AWeaponBase::FullAmmo()
{
    CurrentAmmo = SourceStats.MagazineSize;
}

//void AWeaponBase::Reload(float deltaTime)
//{
//    ReloadTimer += deltaTime;
//    if (SourceStats.ReloadTime <= ReloadTimer)
//    {
//        ReloadTimer = 0;
//        CurrentAmmo = SourceStats.MagazineSize;
//        //isReload = false;
//    }
//}

void AWeaponBase::HandlePickUp(ACharacterBase* character)
{
    UEquipmentComponent* equipComp = character->FindComponentByClass<UEquipmentComponent>();
    if (!equipComp) return;
    equipComp->Equip(this, character->GetMesh());
    ProjectileInstigator = character->GetController();
}

FTransform AWeaponBase::GetProjectileSpawnTransform() const
{
    
    // ソケットが存在する場合
    if (SkeletalMesh && SkeletalMesh->DoesSocketExist(SocketName))
    {
        // ソケットのTransformを取得（ワールド座標系）
        FTransform SocketTransform = SkeletalMesh->GetSocketTransform(SocketName, RTS_World);

        // 射撃方向のオフセットを加える（元のShotDirection相当）
        SocketTransform.ConcatenateRotation(ShotDirection.Quaternion());

        return SocketTransform;
    }

    // ソケットがない場合はアクター位置・回転から生成
    return FTransform(GetActorRotation(), GetActorLocation());
}

void AWeaponBase::PlayMuzzleEffect()
{
    if (MuzzleFlash && SkeletalMesh && SkeletalMesh->DoesSocketExist(SocketName))
    {
        UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, SkeletalMesh, SocketName);
    }
    else if (MuzzleFlash)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, GetActorLocation());
    }
}

void AWeaponBase::UpdateFireTimer(float DeltaTime)
{
    FireIntervalTimer += DeltaTime;
    if (FireIntervalTimer >= FireInterval)
        {
            FireIntervalTimer = FireInterval;
        }
}

void AWeaponBase::Drop()
{
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    if (WeaponStateMachine) WeaponStateMachine->ChangeStateByClass(UWeaponState_Idle::StaticClass());
    OnDropped.Broadcast();
}

