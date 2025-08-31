// Fill out your copyright notice in the Description page of Project Settings.
#include "PickUpComponent.h"
#include "EquipmentComponent.h"
#include "WeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UPickUpComponent::UPickUpComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionProfileName(TEXT("OverlapAll"));
	SetCollisionObjectType(ECC_GameTraceChannel2);
	SetSimulatePhysics(false);
	DelaySeconds_EnablePickup = 0.5f;

}

void UPickUpComponent::PickedUpByCharacter(ACharacterBase* character)
{
	OnPickUp.Broadcast(character);//登録されたイベント呼ぶ
	UKismetSystemLibrary::PrintString(this, "Call PickUp", true, true, FColor::Cyan, 2.f, TEXT("None"));
	DisablePickup();
}

void UPickUpComponent::EnablePickup()
{
	UE_LOG(LogTemp, Warning, TEXT("EnablePickup called."));

	// コリジョン
	SetCollisionProfileName(TEXT("OverlapAll"));
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//イベントをクリア&再登録
	OnComponentBeginOverlap.Clear();
	OnComponentBeginOverlap.AddDynamic(this, &UPickUpComponent::OnBoxBeginOverlap);

	//状態ログだけ出す
	UE_LOG(LogTemp, Warning, TEXT("Collision Enabled: %d"), GetCollisionEnabled());
	UE_LOG(LogTemp, Warning, TEXT("GenerateOverlapEvents: %d"), GetGenerateOverlapEvents());
}



void UPickUpComponent::DisablePickup()
{
	OnComponentBeginOverlap.RemoveAll(this);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);

}


// Called when the game starts
void UPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AWeaponBase* Weapon = Cast<AWeaponBase>(GetOwner()))
	{
		Weapon->OnDropped.AddDynamic(this, &UPickUpComponent::HandleOwnerDropped);
	}

	EnablePickup();

}

void UPickUpComponent::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UKismetSystemLibrary::PrintString(this, "Check PickUp", true, true, FColor::Cyan, 2.f, TEXT("None"));
	ATopDownGame_1Character* character = Cast<ATopDownGame_1Character>(OtherActor);
	if (character)
	{
		PickedUpByCharacter(character);
	}
}

void UPickUpComponent::HandleOwnerDropped()
{
	//タイマーがあればクリア
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_EnablePickup);

	if (DelaySeconds_EnablePickup <= 0.f)
	{
		EnablePickup();
	}
	else
	{
		//ディレイ後に呼ぶ
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_EnablePickup, this, &UPickUpComponent::EnablePickup, DelaySeconds_EnablePickup, false);
	}
}


