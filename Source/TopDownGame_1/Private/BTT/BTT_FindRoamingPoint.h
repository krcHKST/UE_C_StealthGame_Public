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
	//�Q�l:https://tech.spark-creative.co.jp/entry/2024/04/30/154401
public:
	
	explicit UBTT_FindRoamingPoint(FObjectInitializer const& ObjectInitializer);
	// �^�X�N���s���e
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
private:

	// �u���b�N�{�[�h�ɕۊǂ���p�̃L�[���
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector RoamingPointVectorKey;
};
