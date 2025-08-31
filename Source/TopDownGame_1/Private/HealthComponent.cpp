// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("HealthComponent BeginPlay: MaxHealth is %.2f"), MaxHealth), true, true, FColor::Green, 10.f);

	Health = MaxHealth;

	// HealthがMaxHealthと同じ値で初期化されたか確認
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("HealthComponent BeginPlay: Initial Health set to %.2f"), Health), true, true, FColor::Green, 10.f);

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);

	//初期化用
	OnHealthChanged.Broadcast(Health, MaxHealth);
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}

	float ActualDamage = Damage;
	AActor* MyOwner = GetOwner();

	//Ownerのインターフェースをチェック
	if (MyOwner && MyOwner->Implements<UDamageModifierInterface>())
	{
		//ダメージ計算を委譲
		const FDamageEvent DamageEvent(DamageType->GetClass());
		ActualDamage = IDamageModifierInterface::Execute_ApplyDamageModification(MyOwner, Damage, DamageEvent, InstigatedBy, DamageCauser);
	}
	Health = FMath::Clamp(Health - ActualDamage, 0.0f, MaxHealth);
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Damage Applied: %.2f (Original: %.2f), Current Health: %.2f"), ActualDamage, Damage, Health), true, true, FColor::Cyan, 5.f);

	OnHealthChanged.Broadcast(Health, MaxHealth);
	if (Health <= 0.0f) {
		OnDeath();
	}
}

bool UHealthComponent::OnDeath() {
	UKismetSystemLibrary::PrintString(this,TEXT("Run OnDeath"),true,true,FColor::Cyan,5.f);

	if (DeathEvent.IsBound())
	{
		DeathEvent.Execute();

		UKismetSystemLibrary::PrintString(this,TEXT("Run DeathEvent"),true,true,FColor::Cyan,5.f);
		return true;
	}

	return false;
}

float UHealthComponent::GetHealthPercent() const
{
	if (MaxHealth <= 0.0f || Health <= 0.0f)
	{
		return 0.0f;
	}

	return Health / MaxHealth;
}
