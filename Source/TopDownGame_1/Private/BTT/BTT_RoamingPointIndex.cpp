// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT/BTT_RoamingPointIndex.h"
#include "TopDownGame_1/EnemyBaseCharacter.h"
#include "TopDownGame_1/AIC_EnemyBase.h"
#include "TopDownGame_1/Public/EnemyRoamingPoint.h"

UBTT_RoamingPointIndex::UBTT_RoamingPointIndex(FObjectInitializer const& ObjectInitializer)
{
	// BehaviorTree‚Ìƒ^ƒXƒN—“‚É•\Ž¦‚·‚é–¼‘O
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
				// œpœj—v‘f‚Ì‘S‚Ä‚Ì”‚ðŽæ“¾
				int32 const AllPoints = enemy->GetRoamingPointActor()->PointNum();
				// —v‘f”‚ÌÅ’á’l
				int32 const MinIndex = 0;
				// —v‘f”‚ÌÅ‚’l
				int32 const MaxIndex = AllPoints - 1;
				// Œ»Ý‚Ì—v‘f”Ô†
				int32 Index = BB_Component->GetValueAsInt(GetSelectedBlackboardKey());

				// ‹t•ûŒü‚ªtrue‚Ìê‡
				if (bDirectional)
				{
					// Œ»Ý‚Ì—v‘f”Ô†‚ªÅ‘å’l && ‡•ûŒü‚Éi‚ñ‚Å‚¢‚½‚ç 
					if (Index >= MaxIndex && Direction == EDirectionType::Forward)
					{
						// ‹t•ûŒü‚Ö
						Direction = EDirectionType::Reverse;
					}
					// Œ»Ý‚Ì—v‘f”Ô†‚ªÅ¬’l && ‹t•ûŒü‚Éi‚ñ‚Å‚¢‚½‚ç
					else if (Index == MinIndex && Direction == EDirectionType::Reverse)
					{
						// ‡•ûŒü‚Ö
						Direction = EDirectionType::Forward;
					}
				}

				// ‡•ûŒü‚Ìê‡‚Í—v‘f‡‚Éƒ|ƒCƒ“ƒg”Ô†‚ðÝ’è@(0¨1¨2)
				// ‹t•ûŒü‚Ìê‡‚Í—v‘f”Ô†‚ð‹t‚Éƒ|ƒCƒ“ƒg”Ô†‚ðÝ’è@(2¨1¨0)
				BB_Component->SetValueAsInt(GetSelectedBlackboardKey(),
					(Direction == EDirectionType::Forward ? ++Index : --Index) % AllPoints);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}