// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class TOPDOWNGAME_1_API AProjectileBase : public AActor
{
    GENERATED_BODY()
    /** Sphere collision component */
    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
    USphereComponent* CollisionComp;
    UPROPERTY()
    AActor* ParentActor;
public:
    // Sets default values for this actor's properties
    AProjectileBase();
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UProjectileMovementComponent* ProjectileMovement;
    UPROPERTY(EditAnywhere)
    float DamageAmount;
    UPROPERTY(EditAnywhere)
    class UParticleSystem* ImpactEffect;
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* StaticMesh;
    UFUNCTION()
    void OnProjectileStopCallback(const FHitResult& ImpactResult);
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
public:
    UPROPERTY()
    AController* DamageInstigator;
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    // Object Pool System
    UPROPERTY()
    TWeakObjectPtr<class AProjectilePoolManager> PoolManager;
    void DeactivateProjectile();
    void ReturnToPool();
    virtual void OnActivate();
    virtual void OnDeactivate();
    /** Activate projectile from pool */
    void ActivateProjectile(FVector SpawnLocation, FRotator SpawnRotation);
    //get set
    USphereComponent* GetCollisionComp() const { return CollisionComp; }
    /** Returns ProjectileMovement subobject **/
    UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
    void SetDamageAmount(float newAmount) { DamageAmount = newAmount; }
    void SetDamageInstigator(AController* instigator) { DamageInstigator = instigator; }
    void SetParentActor(AActor* parent) { ParentActor = parent; }
    void SpawnImpactEffects(FTransform transform);
    /** Returns own class type */
    UClass* GetProjectileClass() const { return GetClass(); }
};
