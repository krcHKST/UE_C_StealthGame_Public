// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/Actor.h"
#include "AIC_EnemyBase.generated.h"


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPerceptionUpdatedDelegate, TArray, UpdatedActors);
/**
 * 
 */
UCLASS()
class TOPDOWNGAME_1_API AAIC_EnemyBase : public AAIController
{
	GENERATED_BODY()

public:
	AAIC_EnemyBase(const class FObjectInitializer& ObjectInitializer);

	UFUNCTION(BluePrintCallable)
	void SetPlayerKey(ACharacter* player);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PerceptionExtended")
	void GetHeadLocRot(FVector& out_location, FRotator& out_rotation) const;

	virtual void GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const override;

	UFUNCTION()
	void SenseStuff(const TArray<AActor*> &SenseActors);
	UFUNCTION()
	class ATopDownGame_1Character* GetPlayerKey();

	UPROPERTY()
	UBehaviorTreeComponent* BehaviorComp;

	UPROPERTY()
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName PlayerKeyName;
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName FindKeyName;
	UPROPERTY(EditAnywhere, Category = "Perceptions")
	float SightRadius;
	UPROPERTY(EditAnywhere,Category = "Perceptions")
	float LoseSightRadius;
	UPROPERTY(EditAnywhere, Category = "Perceptions")
	float AngleDegrees;

	UPROPERTY(EditAnywhere)
	class AEnemyBaseCharacter* OwnerEnemy;
	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionComponent* AIPerception;
	UPROPERTY(EditDefaultsOnly)
	UAISenseConfig_Sight* SightConfig;
protected:
	// AIControllerÇÃPawnèäéù
	virtual void OnPossess(class APawn* InPawn) override;

	// AIControllerÇÃPawnèäéùâèú
	virtual void OnUnPossess() override;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	class UBehaviorTree* BehaviorTree;

	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
};
