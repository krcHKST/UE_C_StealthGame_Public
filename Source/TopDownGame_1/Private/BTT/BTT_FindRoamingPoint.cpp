// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT/BTT_FindRoamingPoint.h"
#include "TopDownGame_1/Public/EnemyRoamingPoint.h"
#include "TopDownGame_1/EnemyBaseCharacter.h"
#include "TopDownGame_1/AIC_EnemyBase.h"

UBTT_FindRoamingPoint::UBTT_FindRoamingPoint(FObjectInitializer const& ObjectInitializer)
{
	// BehaviorTree�̃^�X�N���ɕ\�����閼�O
	NodeName = TEXT("Find Roaming Point");
}

EBTNodeResult::Type UBTT_FindRoamingPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIC_EnemyBase* AIC = Cast<AAIC_EnemyBase>(OwnerComp.GetAIOwner()))
	{
		if (auto* BB_Component = OwnerComp.GetBlackboardComponent())
		{
			// ���݂̜p�j�v�f�ԍ�
			int32 index = BB_Component->GetValueAsInt(GetSelectedBlackboardKey());

			if (AEnemyBaseCharacter* Enemy = Cast<AEnemyBaseCharacter>(AIC->GetPawn()))
			{
				if (AEnemyRoamingPoint* RoamingPointActor = Enemy->GetRoamingPointActor()) 
				{
					// �v�f�ԍ����瑊�Έʒu���擾
					auto const roamingPoint = RoamingPointActor->GetRoamingPoint(index);
					// ���Έʒu���烏�[���h�ʒu�֕ϊ�
					auto const targetPoint = RoamingPointActor->GetActorTransform().TransformPosition(roamingPoint);
					// �u���b�N�{�[�h�̜p�j�ʒu�L�[�ɍ��W��ݒ�
					BB_Component->SetValueAsVector(RoamingPointVectorKey.SelectedKeyName, targetPoint);

					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

