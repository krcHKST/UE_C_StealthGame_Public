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
	//�Q�l:https://tech.spark-creative.co.jp/entry/2024/04/30/154401
public:
	explicit UBTT_RoamingPointIndex(FObjectInitializer const& ObjectInitializer);
	// �^�X�N���s���e
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

private:
	// ���̃|�C���g�ւ̕��������߂܂��B
	// Forward�͔z��v�f���ɁAReverse�͔z��v�f�̋t���ł�
	enum class EDirectionType { Forward, Reverse };

	// �������͏�������
	EDirectionType Direction = EDirectionType::Forward;

	// �������A�t���������߂�bool�l
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bDirectional = false;
};
