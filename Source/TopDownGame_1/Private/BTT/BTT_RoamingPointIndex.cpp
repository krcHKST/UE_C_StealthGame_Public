// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT/BTT_RoamingPointIndex.h"
#include "TopDownGame_1/EnemyBaseCharacter.h"
#include "TopDownGame_1/AIC_EnemyBase.h"
#include "TopDownGame_1/Public/EnemyRoamingPoint.h"

UBTT_RoamingPointIndex::UBTT_RoamingPointIndex(FObjectInitializer const& ObjectInitializer)
{
	// BehaviorTree�̃^�X�N���ɕ\�����閼�O
	NodeName = TEXT("Roaming Point Index");
}

EBTNodeResult::Type UBTT_RoamingPointIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIC_EnemyBase* AIC = Cast<AAIC_EnemyBase>(OwnerComp.GetAIOwner()))
	{
		if (AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(AIC->GetPawn()))
		{
			if (auto* const BB_Component = OwnerComp.GetBlackboardComponent())
			{
				// �p�j�v�f�̑S�Ă̐����擾
				int32 const AllPoints = enemy->GetRoamingPointActor()->PointNum();
				// �v�f���̍Œ�l
				int32 const MinIndex = 0;
				// �v�f���̍ō��l
				int32 const MaxIndex = AllPoints - 1;
				// ���݂̗v�f�ԍ�
				int32 Index = BB_Component->GetValueAsInt(GetSelectedBlackboardKey());

				// �t������true�̏ꍇ
				if (bDirectional)
				{
					// ���݂̗v�f�ԍ����ő�l && �������ɐi��ł����� 
					if (Index >= MaxIndex && Direction == EDirectionType::Forward)
					{
						// �t������
						Direction = EDirectionType::Reverse;
					}
					// ���݂̗v�f�ԍ����ŏ��l && �t�����ɐi��ł�����
					else if (Index == MinIndex && Direction == EDirectionType::Reverse)
					{
						// ��������
						Direction = EDirectionType::Forward;
					}
				}

				// �������̏ꍇ�͗v�f���Ƀ|�C���g�ԍ���ݒ�@(0��1��2)
				// �t�����̏ꍇ�͗v�f�ԍ����t�Ƀ|�C���g�ԍ���ݒ�@(2��1��0)
				BB_Component->SetValueAsInt(GetSelectedBlackboardKey(),
					(Direction == EDirectionType::Forward ? ++Index : --Index) % AllPoints);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}