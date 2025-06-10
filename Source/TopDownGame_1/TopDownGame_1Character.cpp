// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownGame_1Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "WeaponBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ATopDownGame_1Character::ATopDownGame_1Character() : Super()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Set Mesh
	SkeletalMesh = GetMesh();
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
}

void ATopDownGame_1Character::BeginPlay() 
{
	Super::BeginPlay();

	CurrentLife = MaxLife;
	CurrentStamina = MaxStamina;

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
}

void ATopDownGame_1Character::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	
}


void ATopDownGame_1Character::SetHasWeapon(bool NewHasWeapon)
{
	bHasWeapon = NewHasWeapon;
}

//float ATopDownGame_1Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//	return 0.0f;
//}

void ATopDownGame_1Character::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATopDownGame_1Character::HandleOnDestroyed(AActor* DestroyedActor)
{
	Super::HandleOnDestroyed(DestroyedActor);
	//UGameplayStatics::OpenLevel(this, "GameOverMap");
}

void ATopDownGame_1Character::DeathReaction()
{
	Super::DeathReaction();
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->DisableInput(PC);
	}

	USkeletalMeshComponent* meshComp = GetMesh();
	if (!meshComp) return;

	// ラグドール化
	meshComp->SetCollisionProfileName(TEXT("Ragdoll"));
	meshComp->SetSimulatePhysics(true);
	// ルートのコントロールも停止
	meshComp->SetAllBodiesSimulatePhysics(true);
	meshComp->WakeAllRigidBodies();
	meshComp->bBlendPhysics = true;

	// コントローラーを無効化
	DetachFromControllerPendingDestroy();

	// カプセルを無効化
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle TimerHandle;//レベル読み込みなおす
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATopDownGame_1Character::ReloadLevel, 5.0f, false);
}

void ATopDownGame_1Character::ReloadLevel()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FName CurrentLevelName = *World->GetMapName();
		FString CleanLevelName = FPackageName::GetShortName(CurrentLevelName);
		UGameplayStatics::OpenLevel(this, FName(*CleanLevelName));
	}
}