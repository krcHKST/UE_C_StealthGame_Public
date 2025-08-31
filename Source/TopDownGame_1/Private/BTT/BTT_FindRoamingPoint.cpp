// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT/BTT_FindRoamingPoint.h"
#include "TopDownGame_1/Public/EnemyRoamingPoint.h"
#include "TopDownGame_1/EnemyBaseCharacter.h"
#include "TopDownGame_1/AIC_EnemyBase.h"
#include "EnemyRoamingPointComponent.h"
#include "RoomBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_FindRoamingPoint::UBTT_FindRoamingPoint(FObjectInitializer const& ObjectInitializer)
{
	// BehaviorTreeのタスク欄に表示する名前
	NodeName = TEXT("Find Roaming Point");
}

EBTNodeResult::Type UBTT_FindRoamingPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AIController取得
    if (AAIC_EnemyBase* AIC = Cast<AAIC_EnemyBase>(OwnerComp.GetAIOwner()))
    {
        // Blackboard取得
        if (UBlackboardComponent* BB_Component = OwnerComp.GetBlackboardComponent())
        {
            // 現在の徘徊要素番号（UBTT_RoamingPointIndexで管理）
            int32 index = BB_Component->GetValueAsInt(GetSelectedBlackboardKey());

            // Enemy取得
            if (AEnemyBaseCharacter* Enemy = Cast<AEnemyBaseCharacter>(AIC->GetPawn()))
            {
                // 親Roomのチェック
                if (ARoomBase* Room = Enemy->GetParentRoom())
                {
                    // 部屋内のすべての EnemyRoamingPointComponent を取得
                    TArray<UEnemyRoamingPointComponent*> AllPointsInRoom;
                    Room->GetComponents<UEnemyRoamingPointComponent>(AllPointsInRoom);
                    // EnemyのRoamingIDを取得
                    int32 const EnemyRoamingID = Enemy->GetRoamingID();

                    // RoamingIDが一致するポイントのみ抽出する
                    TArray<UEnemyRoamingPointComponent*> RoamingPoints;
                    for (UEnemyRoamingPointComponent* Point : AllPointsInRoom)
                    {
                        if (Point && Point->GetRoamingID() == EnemyRoamingID)
                        {
                            RoamingPoints.Add(Point);
                        }
                    }

                    if (RoamingPoints.IsValidIndex(index))
                    {
                        // ワールド座標をBBにセット
                        FVector TargetPoint = RoamingPoints[index]->GetComponentLocation();
                        BB_Component->SetValueAsVector(RoamingPointVectorKey.SelectedKeyName, TargetPoint);

                        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                        return EBTNodeResult::Succeeded;
                    }
                }
            }
        }
    }

    return EBTNodeResult::Failed;
}