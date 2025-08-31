// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIC_EnemyBase.generated.h"

class UBehaviorTree;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class UAISenseConfig_Sight;
class AEnemyBaseCharacter;

/**
 * 敵キャラクターのベースとなるAIコントローラー
 */
UCLASS()
class TOPDOWNGAME_1_API AAIC_EnemyBase : public AAIController
{
	GENERATED_BODY()

public:
	AAIC_EnemyBase(const FObjectInitializer& ObjectInitializer);
	UFUNCTION()
	FName GetPlayerKeyName() { return PlayerKeyName; };
protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	// AIの目の位置設定する
	virtual void GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const override;

	UFUNCTION()
	void OnTargetSensed(AActor* SensedActor, FAIStimulus Stimulus);

	// 頭部の座標と回転を取得するイベント
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Perception")
	void GetHeadSocketLocationAndRotation(FVector& OutLocation, FRotator& OutRotation) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBehaviorTreeComponent> BehaviorComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBlackboardComponent> BlackboardComp;

	// 知覚機能コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	// 視覚に関するパラメータ（BPで設定）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Perception", meta = (DisplayName = "Sight Radius"))
	float SightRadius = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Perception", meta = (DisplayName = "Lose Sight Radius"))
	float LoseSightRadius = 2500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Perception", meta = (DisplayName = "Vision Angle Degrees"))
	float PeripheralVisionAngleDegrees = 90.0f;

	// ブラックボードで使用するキーの名前
	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName PlayerKeyName = "TargetActor";

	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName FindKeyName = "bIsFind";

private:

	TObjectPtr<AEnemyBaseCharacter> OwnerEnemy;
};