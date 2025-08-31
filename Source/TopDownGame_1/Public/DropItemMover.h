#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DropItemMover.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOPDOWNGAME_1_API UDropItemMover : public UActorComponent
{
    GENERATED_BODY()

public:
    UDropItemMover();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
    void ResetMovement();

    UFUNCTION(BlueprintCallable)
    void StopMovement();

private:
    UPrimitiveComponent* TargetMesh;
    FMatrix CurrentMatrix;
    FVector StartLocation;
    FVector Velocity;
    bool bOnGround;
    bool bIsActive;
    UFUNCTION()
    void UpdateMovement(float DeltaTime);
    UFUNCTION()
    void HandlePickedUp(class ACharacterBase* PickUpCharacter);

    UFUNCTION()
    void HandleDropped();

    FVector GroundLocation;
    float FloatingTime;
    UPROPERTY(EditAnywhere, Category = "Movement", meta = (EditCondition = "bOnGround"))
    float FloatingLength = 10.0f;//è„â∫ÇÃïù

    UPROPERTY(EditAnywhere, Category = "Movement", meta = (EditCondition = "bOnGround"))
    float FloatingSpeed = 1.5f;//è„â∫ÇÃë¨Ç≥

    /*UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);*/
};
