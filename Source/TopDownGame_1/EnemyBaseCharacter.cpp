// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TopDownGame_1Character.h"
#include "AIC_EnemyBase.h"
#include "HealthComponent.h"
#include "WeaponBase.h"
#include "EquipmentComponent.h"
#include "TopDownGame_1/Public/EnemyRoamingPoint.h"

// Sets default values
AEnemyBaseCharacter::AEnemyBaseCharacter() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	//PerceptionSightに移行、未使用
	//PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	//// 視野
	//PawnSensingComp->SetPeripheralVisionAngle(60.f);
	//// 見える範囲
	//PawnSensingComp->SightRadius = 2000;
	//PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyBaseCharacter::OnSeePlayer);
}

void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (RoamingPointActor == nullptr) {

	}

	//HealthComponent->DeathEvent.BindUObject(this,&AEnemyBaseCharacter::Death);
}

void AEnemyBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EnemyState == EEnemyState::Find)Fire();
}

void AEnemyBaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	EquipmentComponent->DiscardWeapon();
}

float AEnemyBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator,DamageCauser);
	return ActualDamage;
}

void AEnemyBaseCharacter::OnSeePlayer(ACharacter* character)
{
	//AAIC_EnemyBase* AIController = Cast<AAIC_EnemyBase>(GetController());
	//// プレイヤー
	//ATopDownGame_1Character* player = Cast<ATopDownGame_1Character>(character);

	//if (AIController && player)
	//{
	//	// AIControllerにプレイヤー情報を設定
	//	AIController->SetPlayerKey(player);
	//}

	//// 視野に入ったら画面に"See"と表示
	//UKismetSystemLibrary::PrintString(this, "See", true, true, FColor::Blue, 2.f);
}

AEnemyRoamingPoint* AEnemyBaseCharacter::GetRoamingPointActor()
{
	if (RoamingPointActor != nullptr)
		return RoamingPointActor;
	else return nullptr;
}