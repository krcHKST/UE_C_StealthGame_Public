// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT/BTT_FindRoamingPoint.h"
#include "TopDownGame_1/Public/EnemyRoamingPoint.h"
#include "TopDownGame_1/EnemyBaseCharacter.h"
#include "TopDownGame_1/AIC_EnemyBase.h"

UBTT_FindRoamingPoint::UBTT_FindRoamingPoint(FObjectInitializer const& ObjectInitializer)
{
	// BehaviorTreeのタスク欄に表示する名前
	NodeName = TEXT("Find Roaming Point");
}

EBTNodeResult::Type UBTT_FindRoamingPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIC_EnemyBase* AIC = Cast<AAIC_EnemyBase>(OwnerComp.GetAIOwner()))
	{
		if (auto* BB_Component = OwnerComp.GetBlackboardComponent())
		{
			// 現在の徘徊要素番号
			int32 index = BB_Component->GetValueAsInt(GetSelectedBlackboardKey());

			if (AEnemyBaseCharacter* Enemy = Cast<AEnemyBaseCharacter>(AIC->GetPawn()))
			{
				if (AEnemyRoamingPoint* RoamingPointActor = Enemy->GetRoamingPointActor()) 
				{
					// 要素番号から相対位置を取得
					auto const roamingPoint = RoamingPointActor->GetRoamingPoint(index);
					// 相対位置からワールド位置へ変換
					auto const targetPoint = RoamingPointActor->GetActorTransform().TransformPosition(roamingPoint);
					// ブラックボードの徘徊位置キーに座標を設定
					BB_Component->SetValueAsVector(RoamingPointVectorKey.SelectedKeyName, targetPoint);

					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

