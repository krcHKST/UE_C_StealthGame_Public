// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine/DamageEvents.h"
#include "ProjectilePoolManager.h"

AProjectileBase::AProjectileBase()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComp->CanCharacterStepUpOn = ECB_No;

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
    StaticMesh->SetupAttachment(CollisionComp);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();

    if (DamageInstigator != nullptr)
        ParentActor = DamageInstigator->GetPawn();

    CollisionComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
    ProjectileMovement->OnProjectileStop.AddDynamic(this, &AProjectileBase::OnProjectileStopCallback);

    // ProjectilePoolManager取得
    if (PoolManager.Get())return;
    AActor* const foundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AProjectilePoolManager::StaticClass());
    if (foundActor) {
        AProjectilePoolManager* const poolManager = Cast<AProjectilePoolManager>(foundActor);
        if (poolManager)PoolManager = poolManager;
    }
}

void AProjectileBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    const FRotator hitRotator = GetVelocity().ToOrientationRotator();
    const FTransform hitTransform(hitRotator, Hit.Location);

    if (OtherComp != nullptr && OtherComp->IsSimulatingPhysics())
    {
        OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
        SpawnImpactEffects(hitTransform);
        ReturnToPool();
        return;
    }
    else if (OtherComp == nullptr)
    {
        ReturnToPool();
        return;
    }

    if (OtherActor == this) return;
    if (ParentActor != nullptr && OtherActor == ParentActor) return;

    if (Cast<ACharacter>(OtherActor))
    {
        if (DamageInstigator == nullptr)//無かったら仮でプレイヤーを入れる
            DamageInstigator = GetWorld()->GetFirstPlayerController();
        if (DamageInstigator == nullptr)
            return;

        UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, DamageInstigator, this, UDamageType::StaticClass());
        SpawnImpactEffects(hitTransform);
        ReturnToPool();
    }
}

void AProjectileBase::DeactivateProjectile()
{
    ProjectileMovement->StopMovementImmediately();
    SetActorEnableCollision(false);
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    OnDeactivate();
}

void AProjectileBase::ActivateProjectile(FVector SpawnLocation, FRotator SpawnRotation)
{
    SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
    SetActorEnableCollision(true);
    SetActorHiddenInGame(false);
    SetActorTickEnabled(true);

    ProjectileMovement->StopMovementImmediately();
    ProjectileMovement->SetUpdatedComponent(CollisionComp);
    ProjectileMovement->Velocity = SpawnRotation.Vector() * ProjectileMovement->InitialSpeed;
    ProjectileMovement->Activate(true);
    OnActivate();
}

void AProjectileBase::ReturnToPool()
{
    if (PoolManager.IsValid())
    {
        DeactivateProjectile();
        PoolManager->ReturnProjectileToPool(this);
    }
    else
    {
        Destroy();
    }
}

void AProjectileBase::SpawnImpactEffects(FTransform transform)
{
    if (ImpactEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, transform);
    }
}

void AProjectileBase::OnProjectileStopCallback(const FHitResult& ImpactResult)
{
    ReturnToPool();
}

void AProjectileBase::OnActivate()
{
    // オーバーライドして使う
}

void AProjectileBase::OnDeactivate()
{
    // オーバーライドして使う
}