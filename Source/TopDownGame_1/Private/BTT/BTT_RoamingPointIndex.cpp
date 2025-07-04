// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT/BTT_RoamingPointIndex.h"
#include "TopDownGame_1/EnemyBaseCharacter.h"
#include "TopDownGame_1/AIC_EnemyBase.h"
#include "TopDownGame_1/Public/EnemyRoamingPoint.h"

UBTT_RoamingPointIndex::UBTT_RoamingPointIndex(FObjectInitializer const& ObjectInitializer)
{
	// BehaviorTreeのタスク欄に表示する名前
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
				// 徘徊要素の全ての数を取得
				int32 const AllPoints = enemy->GetRoamingPointActor()->PointNum();
				// 要素数の最低値
				int32 const MinIndex = 0;
				// 要素数の最高値
				int32 const MaxIndex = AllPoints - 1;
				// 現在の要素番号
				int32 Index = BB_Component->GetValueAsInt(GetSelectedBlackboardKey());

				// 逆方向がtrueの場合
				if (bDirectional)
				{
					// 現在の要素番号が最大値 && 順方向に進んでいたら 
					if (Index >= MaxIndex && Direction == EDirectionType::Forward)
					{
						// 逆方向へ
						Direction = EDirectionType::Reverse;
					}
					// 現在の要素番号が最小値 && 逆方向に進んでいたら
					else if (Index == MinIndex && Direction == EDirectionType::Reverse)
					{
						// 順方向へ
						Direction = EDirectionType::Forward;
					}
				}

				// 順方向の場合は要素順にポイント番号を設定　(0→1→2)
				// 逆方向の場合は要素番号を逆にポイント番号を設定　(2→1→0)
				BB_Component->SetValueAsInt(GetSelectedBlackboardKey(),
					(Direction == EDirectionType::Forward ? ++Index : --Index) % AllPoints);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}