// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "EquipmentComponent.h"
#include "WeaponBase.h"
#include "HealthComponent.h"
#include "PickUpComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownGame_1/MainGameMode.h"
ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipComp"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	EquipSocketName = NAME_None;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->DeathEvent.BindUObject(this, &ACharacterBase::DeathReaction);
	OnDestroyed.AddDynamic(this, &ACharacterBase::HandleOnDestroyed);

	//初期武器セット
	if (DefaultHoldWeapon) {
		AWeaponBase* holdWeapon = GetWorld()->SpawnActor<AWeaponBase>(DefaultHoldWeapon);
		holdWeapon->GetPickUpComponent()->PickedUpByCharacter(this);
	}
}

void ACharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// レベル遷移が理由でない場合
	if (EndPlayReason != EEndPlayReason::LevelTransition && EndPlayReason != EEndPlayReason::Quit)
	{
		// 何か特別なクリーンアップ処理など
	}
}

void ACharacterBase::Fire()
{
	EquipmentComponent->Fire();
}

void ACharacterBase::EndFire()
{
	EquipmentComponent->EndFire();
}

float ACharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UKismetSystemLibrary::PrintString(this, "Take Damage Character", true, true, FColor::Cyan, 2.f, TEXT("None"));
	return DamageAmount;
}

void ACharacterBase::DeathReaction()
{
	
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);
	EquipmentComponent->Drop();
	AMainGameMode* gameMode = GetWorld()->GetAuthGameMode<AMainGameMode>();
	if (gameMode)gameMode->PawnKilled(this);

}

void ACharacterBase::HandleOnDestroyed(AActor* DestroyedActor)
{
	UE_LOG(LogTemp, Warning, TEXT("HandleOnDestroyed: %s was destroyed!"), *DestroyedActor->GetName());
}