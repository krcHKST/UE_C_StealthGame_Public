// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT/BTT_EnemyAttack.h"
#include "TopDownGame_1/AIC_EnemyBase.h"
#include "TopDownGame_1/EnemyBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTT_EnemyAttack::UBTT_EnemyAttack()
{
    
    NodeName = TEXT("Enemy Attack");
}

EBTNodeResult::Type UBTT_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    //Enemy用タスクのため判定
    AAIC_EnemyBase* AIC = Cast<AAIC_EnemyBase>(OwnerComp.GetAIOwner());
    if (!AIC)
    {
        return EBTNodeResult::Failed;
    }
    AEnemyBaseCharacter* enemyCharacter = Cast<AEnemyBaseCharacter>(AIC->GetPawn());
    if (!enemyCharacter)
    {
        return EBTNodeResult::Failed;
    }

    AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
    if (!TargetActor)
    {
        return EBTNodeResult::Failed;
    }
    //敵キャラクターに実装されてる攻撃
    enemyCharacter->Fire();
    return EBTNodeResult::Succeeded;
}
