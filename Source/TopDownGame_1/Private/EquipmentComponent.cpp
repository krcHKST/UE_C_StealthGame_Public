// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "WeaponComponent.h"
#include "WeaponBase.h"
#include "PickUpComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentComponent::Equip(AWeaponBase* weapon, USkeletalMeshComponent* attachMesh)
{
	AActor* const ownerActor = GetOwner();
	if (ownerActor)
	{
		FString OwnerName = ownerActor->GetName();

		// Print the owner's name on the screen
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Owner: %s"), *OwnerName), true, true, FLinearColor::Green, 15.0f);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("Owner is null!"), true, true, FLinearColor::Red, 5.0f);
		return;
	}

	const ACharacterBase* ownerCharacter = Cast<ACharacterBase>(ownerActor);

	if (EquipWeapon)
	{
		Drop();//Œ»Ý‚ÌŠŽ‘•”õŽÌ‚Ä‚é
	}
	EquipWeapon = weapon;
	EquipWeapon->SetOwner(GetOwner());
	const FName socketName = ownerCharacter->GetEquipSocket();
	if (socketName.IsNone())
	{
		EquipWeapon->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		UKismetSystemLibrary::PrintString(this, TEXT("Socket is None"), true, true, FLinearColor::Red, 5.0f);
	}
	else
	{
		EquipWeapon->AttachToComponent(attachMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, socketName);
	}
	//“o˜^ƒCƒxƒ“ƒgŒÄ‚Ño‚µ
	if (EquipEvent.IsBound())EquipEvent.Execute();
}

//void UEquipmentComponent::Equip(AWeaponBase* weapon, USkeletalMeshComponent* attachMesh, FName socketName)
//{
//	if (EquipWeapon)
//	{
//		EquipWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//	}
//	EquipWeapon = weapon;
//	EquipWeapon->SetOwner(GetOwner());
//	if (socketName.IsNone()) {
//		EquipWeapon->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
//	}
//	else{
//		EquipWeapon->AttachToComponent(attachMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, socketName);
//	}
//	if(EquipEvent.IsBound())
//	EquipEvent.Execute();
//}

void UEquipmentComponent::SwitchWeapon(int num)
{
	//“ñ‚ÂˆÈã•Ší‚ª‚ ‚Á‚½‚çØ‚è‘Ö‚¦‚é‚æ‚¤‚Èˆ—‚Ì—\’è
}

void UEquipmentComponent::Fire()
{
	if (!EquipWeapon)return;
	EquipWeapon->Fire();
}

void UEquipmentComponent::EndFire()
{
	if (!EquipWeapon)return;
	EquipWeapon->EndFire();
}

void UEquipmentComponent::Drop()
{
	if (!EquipWeapon)return;
	EquipWeapon->Drop();
	EquipWeapon = nullptr;
}

void UEquipmentComponent::DiscardWeapon()
{
	if (!EquipWeapon)return;
	EquipWeapon->SetLifeSpan(0.1f);
}

