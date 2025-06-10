// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpComponent.h"
#include "EquipmentComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UPickUpComponent::UPickUpComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetCollisionProfileName(TEXT("OverlapAll"));
	SetCollisionObjectType(ECC_GameTraceChannel2);
}

void UPickUpComponent::PickedUpByCharacter(ACharacterBase* character)
{
	OnPickUp.Broadcast(character);//“o˜^‚³‚ê‚½ƒCƒxƒ“ƒgŒÄ‚Ô
	UKismetSystemLibrary::PrintString(this, "Call PickUp", true, true, FColor::Cyan, 2.f, TEXT("None"));
	// ‚à‚¤ˆê‰ñ”­“®‚µ‚È‚¢‚æ‚¤‚É
	OnComponentBeginOverlap.RemoveAll(this);
}


// Called when the game starts
void UPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UPickUpComponent::OnBoxBeginOverlap);

}

void UPickUpComponent::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UKismetSystemLibrary::PrintString(this, "Check PickUp", true, true, FColor::Cyan, 2.f, TEXT("None"));
	// Checking if it is a First Person Character overlapping
	ATopDownGame_1Character* character = Cast<ATopDownGame_1Character>(OtherActor);
	if (character)
	{
		PickedUpByCharacter(character);
	}
}


