// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "TopDownGame_1Character.h"
#include "AIC_EnemyBase.h"
#include "HealthComponent.h"
#include "WeaponBase.h"
#include "EquipmentComponent.h"
#include "TopDownGame_1/Public/EnemyRoamingPoint.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h" 
#include "Widget/HealthBarWidget.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AEnemyBaseCharacter::AEnemyBaseCharacter() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
	HealthWidgetComponent->SetupAttachment(RootComponent);
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // スクリーン空間に表示
}

void AEnemyBaseCharacter::BeginPlay()
{
	if (bRandomWeapon) {
		if (RandomWeaponCandidates.Num() > 0)//初期武器をランダムなものに変更
		{
			int32 RandIndex = FMath::RandRange(0, RandomWeaponCandidates.Num() - 1);
			DefaultHoldWeapon = RandomWeaponCandidates[RandIndex];
		}
	}

	Super::BeginPlay();

	if (RoamingPointActor == nullptr) {

	}

	UUserWidget* widgetObject = HealthWidgetComponent->GetUserWidgetObject();
	if (widgetObject)
	{
		UHealthBarWidget* healthBarWidget = Cast<UHealthBarWidget>(widgetObject);
		if (healthBarWidget && HealthComponent)
		{
			// WidgetにHealthComponentをセットして接続を完了
			healthBarWidget->SetHealthComponent(HealthComponent);
		}
	}
}

void AEnemyBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (EnemyState == EEnemyState::Find)Fire();
}

void AEnemyBaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	EquipmentComponent->DiscardWeapon();
}

float AEnemyBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    AAIC_EnemyBase* AIC = Cast<AAIC_EnemyBase>(GetController());
    if (AIC && AIC->GetBlackboardComponent())
    {
        //ブラックボードから状態を取得(uintとして受け取って変換)
		EnemyState = (EEnemyState)AIC->GetBlackboardComponent()->GetValueAsEnum(TEXT("AIState"));
        //非発見状態なら
        if (EnemyState != EEnemyState::Find)
        {
            UE_LOG(LogTemp, Warning, TEXT("Stealth Attack! Damage: %f"), ActualDamage);
            //BB更新
            AIC->GetBlackboardComponent()->SetValueAsEnum(TEXT("AIState"), (uint8)EEnemyState::Find);
			if (EventInstigator)
			{
				APawn* instigatorPawn = EventInstigator->GetPawn();
				if (instigatorPawn)
				{
					//ダメージを与えてきたアクター（プレイヤー）をターゲット
					AIC->GetBlackboardComponent()->SetValueAsObject(AIC->GetPlayerKeyName(), instigatorPawn);
				}
			}
        }
    }

    return ActualDamage;
}

void AEnemyBaseCharacter::OnSeePlayer(ACharacter* character)
{
	//処理変更のため現在は未使用
}

AEnemyRoamingPoint* AEnemyBaseCharacter::GetRoamingPointActor()
{
	if (RoamingPointActor != nullptr)
		return RoamingPointActor;
	else return nullptr;
}

float AEnemyBaseCharacter::ApplyDamageModification_Implementation(float BaseDamage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ModifiedDamage = BaseDamage;
		//非発見状態なら
	if (GetBBEnemyState() != EEnemyState::Find)
	{
		ModifiedDamage *= 3;
	}
	return ModifiedDamage;
}

EEnemyState AEnemyBaseCharacter::GetBBEnemyState()
{
	AAIC_EnemyBase* AIC = Cast<AAIC_EnemyBase>(GetController());
	if (AIC && AIC->GetBlackboardComponent())
	{
		//ブラックボードから状態を取得(uintとして受け取って変換)
		EnemyState = (EEnemyState)AIC->GetBlackboardComponent()->GetValueAsEnum(TEXT("AIState"));
		return EnemyState;
	}
	else return EEnemyState::Idle;//ない場合は待機扱い（変わるかも）
}