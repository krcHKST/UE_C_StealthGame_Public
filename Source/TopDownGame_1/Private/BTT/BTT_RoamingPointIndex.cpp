// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT/BTT_RoamingPointIndex.h"
#include "TopDownGame_1/EnemyBaseCharacter.h"
#include "TopDownGame_1/AIC_EnemyBase.h"
#include "RoomBase.h"
#include "Components/ActorComponent.h"
#include "EnemyRoamingPointComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_RoamingPointIndex::UBTT_RoamingPointIndex(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Roaming Point Index");
}

EBTNodeResult::Type UBTT_RoamingPointIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AIController取得
    if (AAIC_EnemyBase* AIC = Cast<AAIC_EnemyBase>(OwnerComp.GetAIOwner()))
    {
        // Enemy取得
        if (AEnemyBaseCharacter* Enemy = Cast<AEnemyBaseCharacter>(AIC->GetPawn()))
        {
            // Blackboard取得
            if (UBlackboardComponent* BB_Component = OwnerComp.GetBlackboardComponent())
            {
                // 親Room取得
                if (ARoomBase* Room = Enemy->GetParentRoom())
                {
                    // Room内のすべての RoamingPointComponent を取得
                    TArray<UEnemyRoamingPointComponent*> AllPointsInRoom;
                    Room->GetComponents<UEnemyRoamingPointComponent>(AllPointsInRoom);
                  
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

                    int32 const AllPoints = RoamingPoints.Num();
                    if (AllPoints <= 0)
                    {
                        // 一致する徘徊ポイントが無ければ失敗
                        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                        return EBTNodeResult::Failed;
                    }

                    int32 const MinIndex = 0;
                    int32 const MaxIndex = AllPoints - 1;

                    // 現在の徘徊要素番号
                    int32 Index = BB_Component->GetValueAsInt(GetSelectedBlackboardKey());

                    // 逆方向チェック
                    if (bDirectional)
                    {
                        if (Index >= MaxIndex && Direction == EDirectionType::Forward)
                        {
                            Direction = EDirectionType::Reverse;
                        }
                        else if (Index <= MinIndex && Direction == EDirectionType::Reverse)
                        {
                            Direction = EDirectionType::Forward;
                        }
                    }

                    // 次の index を設定
                    if (Direction == EDirectionType::Forward)
                    {
                        Index = (Index + 1) % AllPoints;
                    }
                    else
                    {
                        Index = (Index - 1 + AllPoints) % AllPoints; // 負の値を回避
                    }

                    BB_Component->SetValueAsInt(GetSelectedBlackboardKey(), Index);

                    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                    return EBTNodeResult::Succeeded;
                }
            }
        }
    }

    return EBTNodeResult::Failed;
}
