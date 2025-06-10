// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_FindRoamingPoint.generated.h"

/**
 * 
 */
UCLASS()
class UBTT_FindRoamingPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	//参考:https://tech.spark-creative.co.jp/entry/2024/04/30/154401
public:
	
	explicit UBTT_FindRoamingPoint(FObjectInitializer const& ObjectInitializer);
	// タスク実行内容
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
private:

	// ブラックボードに保管する用のキー情報
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector RoamingPointVectorKey;
};
