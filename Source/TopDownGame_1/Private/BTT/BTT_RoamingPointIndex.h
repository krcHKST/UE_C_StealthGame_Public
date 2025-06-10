// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_RoamingPointIndex.generated.h"

/**
 * 
 */
UCLASS()
class UBTT_RoamingPointIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	//参考:https://tech.spark-creative.co.jp/entry/2024/04/30/154401
public:
	explicit UBTT_RoamingPointIndex(FObjectInitializer const& ObjectInitializer);
	// タスク実行内容
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

private:
	// 次のポイントへの方向を決めます。
	// Forwardは配列要素順に、Reverseは配列要素の逆順です
	enum class EDirectionType { Forward, Reverse };

	// 初期化は順方向に
	EDirectionType Direction = EDirectionType::Forward;

	// 順方向、逆方向を決めるbool値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bDirectional = false;
};
