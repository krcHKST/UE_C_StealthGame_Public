#include "AutomaticDoor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CharacterBase.h"

AAutomaticDoor::AAutomaticDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// 左ドアのメッシュ
	LeftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoorMesh"));
	LeftDoorMesh->SetupAttachment(RootComponent);

	// 右ドアのメッシュ
	RightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoorMesh"));
	RightDoorMesh->SetupAttachment(RootComponent);

	// トリガーボリューム
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(RootComponent);
	TriggerVolume->SetBoxExtent(FVector(100.f));
	TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AAutomaticDoor::OnTriggerBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AAutomaticDoor::OnTriggerEnd);

	// 右ドアはこの方向に、左ドアは逆方向に移動
	OpenOffset = FVector(0.f, 150.f, 0.f);
	bShouldOpen = false;
}

void AAutomaticDoor::BeginPlay()
{
	Super::BeginPlay();

	// ゲーム開始時にドアの閉じた位置を保存
	LeftDoorClosedPosition = LeftDoorMesh->GetRelativeLocation();
	RightDoorClosedPosition = RightDoorMesh->GetRelativeLocation();
}

void AAutomaticDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ドアの目標位置を決定
	FVector LeftTargetPosition;
	FVector RightTargetPosition;

	if (bShouldOpen && bCanOpen)
	{
		//開く 左ドアはオフセットの逆方向、右ドアはオフセットの方向に移動
		LeftTargetPosition = LeftDoorClosedPosition - OpenOffset;
		RightTargetPosition = RightDoorClosedPosition + OpenOffset;
	}
	else
	{
		//閉じる
		LeftTargetPosition = LeftDoorClosedPosition;
		RightTargetPosition = RightDoorClosedPosition;
	}

	// 左ドアを目標位置へ滑らかに移動
	FVector NewLeftLocation = FMath::VInterpConstantTo(
		LeftDoorMesh->GetRelativeLocation(),
		LeftTargetPosition,
		DeltaTime,
		OpenSpeed
	);
	LeftDoorMesh->SetRelativeLocation(NewLeftLocation);

	// 右ドアを目標位置へ滑らかに移動
	FVector NewRightLocation = FMath::VInterpConstantTo(
		RightDoorMesh->GetRelativeLocation(),
		RightTargetPosition,
		DeltaTime,
		OpenSpeed
	);
	RightDoorMesh->SetRelativeLocation(NewRightLocation);
}

void AAutomaticDoor::OnTriggerBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ACharacterBase>(OtherActor))
	{
		OverlapCharacterCount++;
		bShouldOpen = true;
	}
}

void AAutomaticDoor::OnTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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