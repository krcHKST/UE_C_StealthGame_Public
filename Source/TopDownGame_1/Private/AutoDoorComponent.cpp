#include "AutoDoorComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CharacterBase.h"

UAutoDoorComponent::UAutoDoorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAutoDoorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!LeftDoorMesh || !RightDoorMesh || !TriggerVolume)
	{
		UE_LOG(LogTemp, Warning, TEXT("AutoDoorComponent: Mesh or Trigger not set!"));
		return;
	}

	//‰ŠúˆÊ’u‚ð•Û‘¶
	LeftDoorClosedPosition = LeftDoorMesh->GetRelativeLocation();
	RightDoorClosedPosition = RightDoorMesh->GetRelativeLocation();

	//ƒgƒŠƒK[“o˜^
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &UAutoDoorComponent::OnTriggerBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &UAutoDoorComponent::OnTriggerEnd);
}

void UAutoDoorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!LeftDoorMesh || !RightDoorMesh) return;

	//–Ú•WˆÊ’u
	FVector LeftTargetPosition = bShouldOpen && bCanOpen ? LeftDoorClosedPosition - OpenOffset : LeftDoorClosedPosition;
	FVector RightTargetPosition = bShouldOpen && bCanOpen ? RightDoorClosedPosition + OpenOffset : RightDoorClosedPosition;
	//•âŠÔ‚µ‚ÄˆÚ“®
	FVector NewLeftLocation = FMath::VInterpConstantTo(LeftDoorMesh->GetRelativeLocation(), LeftTargetPosition, DeltaTime, OpenSpeed);
	FVector NewRightLocation = FMath::VInterpConstantTo(RightDoorMesh->GetRelativeLocation(), RightTargetPosition, DeltaTime, OpenSpeed);

	LeftDoorMesh->SetRelativeLocation(NewLeftLocation);
	RightDoorMesh->SetRelativeLocation(NewRightLocation);
}

void UAutoDoorComponent::OnTriggerBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ACharacterBase>(OtherActor))
	{
		OverlapCharacterCount++;
		bShouldOpen = true;
	}
}

void UAutoDoorComponent::OnTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ACharacterBase>(OtherActor))
	{
		OverlapCharacterCount--;
		if (OverlapCharacterCount <= 0)
		{
			bShouldOpen = false;
			OverlapCharacterCount = 0;
		}
	}
}
