// Fill out your copyright notice in the Description page of Project Settings.

#include "AIC_EnemyBase.h"
#include "TopDownGame_1Character.h"
#include "EnemyBaseCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Private/Enum/EEnemyState.h"


AAIC_EnemyBase::AAIC_EnemyBase(const class FObjectInitializer& ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	// 作成したビヘイビアツリーを設定
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/AI/BT_EnemyBase.BT_EnemyBase"));
	BehaviorTree = BTFinder.Object;

	PlayerKeyName = "Player";
	FindKeyName = "isFind";

}

void AAIC_EnemyBase::BeginPlay()
{
	Super::BeginPlay();

	GetAIPerceptionComponent()->RequestStimuliListenerUpdate();

}

//void AAIC_EnemyBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	
//}

void AAIC_EnemyBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// AIControllerがPawn所持した際にBBとBTを使用
	BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	BehaviorComp->StartTree(*BehaviorTree);

	OwnerEnemy = Cast<AEnemyBaseCharacter>(InPawn);

	SightConfig->SightRadius = OwnerEnemy->SightRadius;
	SightConfig->LoseSightRadius = OwnerEnemy->LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = OwnerEnemy->AngleDegrees;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, SightConfig->GetSenseImplementation(), InPawn);
	
	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AAIC_EnemyBase::SenseStuff);

	UKismetSystemLibrary::PrintString(this, "Enemy Posses", true, true, FColor::Cyan, 2.f, TEXT("None"));

	GetAIPerceptionComponent()->RequestStimuliListenerUpdate();

}

void AAIC_EnemyBase::OnUnPossess()
{
	Super::OnUnPossess();
	BehaviorComp->StopTree();
}

void AAIC_EnemyBase::SenseStuff(const TArray<AActor*>& SenseActors)
{
	SetPlayerKey(Cast<ATopDownGame_1Character>(UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0)));

	if (SenseActors.Num() == 0) return;

	int actorsNum = SenseActors.Num();
	ATopDownGame_1Character* player = Cast<ATopDownGame_1Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("actorsNum: %d"), actorsNum), true, true, FColor::Cyan, 2.f, TEXT("None"));

	if (!player) return;

	for (int i = 0; i < actorsNum; i++)
	{
		if (!SenseActors[i]) continue;

		if (SenseActors[i] == player)
		{
			OwnerEnemy->isFind = true;
			OwnerEnemy->EnemyState = EEnemyState::Find;
			SetPlayerKey(player);
			break;
		}
	}
}


void AAIC_EnemyBase::SetPlayerKey(ACharacter* player)
{
	ensure(BlackboardComp);

	// ブラックボードで作成したPlayerというキーにプレイヤー情報を入れる
	BlackboardComp->SetValueAsObject(PlayerKeyName, player);
	BlackboardComp->SetValueAsBool(FindKeyName, true);
}


ATopDownGame_1Character* AAIC_EnemyBase::GetPlayerKey()
{
	ensure(BlackboardComp);

	return Cast<ATopDownGame_1Character>(BlackboardComp->GetValueAsObject(PlayerKeyName));
}

void AAIC_EnemyBase::GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const
{
	GetHeadLocRot(out_Location,out_Rotation);
}