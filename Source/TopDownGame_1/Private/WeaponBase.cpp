// Fill out your copyright notice in the Description page of Project Settings.
#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "PickUpComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileBase.h"
#include "ProjectilePoolManager.h"
#include "EquipmentComponent.h"

AWeaponBase::AWeaponBase()
{
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    SkeletalMesh->SetupAttachment(RootComponent);

    PickUpComponent = CreateDefaultSubobject<UPickUpComponent>(TEXT("PickUpComp"));
    PickUpComponent->SetupAttachment(RootComponent);
}

void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();

    if (PickUpComponent)
    {
        PickUpComponent->OnPickUp.AddDynamic(this, &AWeaponBase::HandlePickUp);
    }

    // ProjectilePoolManagerŽæ“¾
    if (ProjectilePoolManager)return;
    AActor* const foundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AProjectilePoolManager::StaticClass());
    if (foundActor) {
        AProjectilePoolManager* const poolManager = Cast<AProjectilePoolManager>(foundActor);
        if (poolManager)ProjectilePoolManager = poolManager;
    }
}

void AWeaponBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

}

void AWeaponBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (isFiring)
    {
        FireIntervalTimer += DeltaTime;
        if (FireIntervalTimer >= FireInterval)
        {
            FireIntervalTimer = 0;
            SpawnProjectile();
        }
    }
}

void AWeaponBase::Fire() 
{ 
    isFiring = true;
}

void AWeaponBase::EndFire() 
{
    isFiring = false; 
}

void AWeaponBase::SetFireFlag(bool newFlag)
{
    isFiring = newFlag; 
}

void AWeaponBase::SpawnProjectile()
{
    if (!ProjectilePoolManager || !ProjectileClass) return;

    AProjectileBase* Projectile = ProjectilePoolManager->GetProjectileFromPool(ProjectileClass);
    if (!Projectile) return;

    FVector SpawnLocation = SkeletalMesh->GetSocketLocation(FName("Muzzle"));
    FRotator SpawnRotation = SkeletalMesh->GetSocketRotation(FName("Muzzle"));

    Projectile->SetDamageInstigator(ProjectileInstigator);
    Projectile->SetParentActor(this);
    Projectile->ActivateProjectile(SpawnLocation, SpawnRotation);

    if (MuzzleFlash)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SpawnLocation, SpawnRotation);
    }
}

void AWeaponBase::HandlePickUp(ACharacterBase* character)
{
    UEquipmentComponent* equipComp = character->FindComponentByClass<UEquipmentComponent>();
    if (!equipComp) return;
    equipComp->Equip(this, character->GetMesh());
    ProjectileInstigator = character->GetController();
}

void AWeaponBase::Drop()
{
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    isFiring = false;
    SkeletalMesh->SetSimulatePhysics(true);
}