// Fill out your copyright notice in the Description page of Project Settings.

#include "AIC_EnemyBase.h"
#include "TopDownGame_1Character.h"
#include "EnemyBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Public/Enum/Enemy/EEnemyState.h"

AAIC_EnemyBase::AAIC_EnemyBase(const FObjectInitializer& ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
}

void AAIC_EnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAIC_EnemyBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerEnemy = Cast<AEnemyBaseCharacter>(InPawn);
	if (!OwnerEnemy)
	{
		UE_LOG(LogTemp, Error, TEXT("Possessed Pawn is not an AEnemyBaseCharacter in %s"), *GetName());
		return;// 失敗したらAI設定は行わない
	}

	//BPでBehaviorTreeアセット設定したか確認
	if (BehaviorTree)
	{
		//BB.BT開始
		BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BehaviorComp->StartTree(*BehaviorTree);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BehaviorTree is not assigned in AI Controller: %s"), *GetName());
	}

	if (IsValid(AIPerception))
	{
		//プロパティから視覚パラメータ設定
		SightConfig->SightRadius = this->SightRadius;
		SightConfig->LoseSightRadius = this->LoseSightRadius;
		SightConfig->PeripheralVisionAngleDegrees = this->PeripheralVisionAngleDegrees;

		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

		AIPerception->ConfigureSense(*SightConfig);
		//視覚を主要なSenseとして設定
		AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

		UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, SightConfig->GetSenseImplementation(), InPawn);
		//OnTargetSensedバインド
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AAIC_EnemyBase::OnTargetSensed);

		//更新（これ大事）
		GetAIPerceptionComponent()->RequestStimuliListenerUpdate();
	}
}

void AAIC_EnemyBase::OnUnPossess()
{
	Super::OnUnPossess();

	//ビヘイビアツリー停止
	if (BehaviorComp && BehaviorComp->IsRunning())
	{
		BehaviorComp->StopTree();
	}
}

void AAIC_EnemyBase::GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const
{
	//BP側で目の位置を定義している
	GetHeadSocketLocationAndRotation(out_Location, out_Rotation);
}

void AAIC_EnemyBase::OnTargetSensed(AActor* SensedActor, FAIStimulus Stimulus)
{
	//プレイヤーか確認
	ATopDownGame_1Character* PlayerCharacter = Cast<ATopDownGame_1Character>(SensedActor);
	if (!PlayerCharacter || !BlackboardComp)
	{
		return; 
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		//プレイヤー発見時の処理 
		BlackboardComp->SetValueAsObject(PlayerKeyName, PlayerCharacter);
		BlackboardComp->SetValueAsBool(FindKeyName, true);

		BlackboardComp->SetValueAsEnum(TEXT("AIState"), (uint8)EEnemyState::Find);
		/*if (IsValid(OwnerEnemy))
		{
			OwnerEnemy->EnemyState = EEnemyState::Find;
		}*/
	}
	else
	{
		//プレイヤーを見失った時の処理
		AEnemyBaseCharacter* enemyCharacter = Cast<AEnemyBaseCharacter>(GetPawn());
		if (enemyCharacter)
		{
			enemyCharacter->EndFire();//この処理はAIの挙動が増えたらBTTとして移動するかも
		}
		BlackboardComp->ClearValue(PlayerKeyName);
		BlackboardComp->SetValueAsBool(FindKeyName, false);

		BlackboardComp->SetValueAsEnum(TEXT("AIState"), (uint8)EEnemyState::Idle);
	}
}