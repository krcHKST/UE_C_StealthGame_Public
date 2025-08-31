#include "DropItemMover.h"
#include "PickUpComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WeaponBase.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UDropItemMover::UDropItemMover()
{
    PrimaryComponentTick.bCanEverTick = true;
    //bHasInitialized = false;
    bIsActive = false;
    bOnGround = false;
    Velocity = FVector::ZeroVector;

    GroundLocation = FVector::ZeroVector;
    FloatingTime = 0.f;
}

// Called when the game starts
void UDropItemMover::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (Owner)
    {
        TargetMesh = Owner->FindComponentByClass<USkeletalMeshComponent>(); // 明示的にスケルタル
        if (!TargetMesh)
        {
            UE_LOG(LogTemp, Error, TEXT("DropItemMover: No SkeletalMeshComponent found."));
            return;
        }

        AActor* Parent = Owner->GetAttachParentActor();
        if (!Parent)
        {
            bIsActive = true;
            ResetMovement();
        }

        // ピックアップイベントバインド
        if (UPickUpComponent* PickUpComp = Owner->FindComponentByClass<UPickUpComponent>())
        {
            PickUpComp->OnPickUp.AddDynamic(this, &UDropItemMover::HandlePickedUp);
        }

        if (AWeaponBase* Weapon = Cast<AWeaponBase>(Owner))
        {
            Weapon->OnDropped.AddDynamic(this, &UDropItemMover::HandleDropped);
        }
        else
        {
            UKismetSystemLibrary::PrintString(this, "DropItemMover: NotMatch Owner for Weapon", true, true, FColor::Red, 2.f, TEXT("None"));
        }
    }
}

// Called every frame
void UDropItemMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsActive)
    {
        UpdateMovement(DeltaTime);
    }
}

void UDropItemMover::ResetMovement()
{
    if (!TargetMesh) return;

    AActor* Owner = GetOwner();
    if (!Owner) return;

    StartLocation = Owner->GetActorLocation();
    FRotator InitRot = Owner->GetActorRotation();

    CurrentMatrix = FRotationMatrix::Make(InitRot);
    CurrentMatrix.SetOrigin(StartLocation);

    // ランダム方向にふわっと浮かせる初速
    FVector RandomPush = FVector(
        FMath::FRandRange(-80.f, 80.f),
        FMath::FRandRange(-80.f, 80.f),
        FMath::FRandRange(200.f, 350.f)
    );

    Velocity = RandomPush;

    FloatingTime = 0.f;
    bOnGround = false;
    bIsActive = true;

    //TargetMesh->SetSimulatePhysics(true);

    UKismetSystemLibrary::PrintString(this, "DropItemMover: Movement reset and activated.", true, true, FColor::Cyan, 2.f, TEXT("None"));
}

void UDropItemMover::StopMovement()
{
    bIsActive = false;
    bOnGround = false;
    Velocity = FVector::ZeroVector;

    /*if (TargetMesh)
    {
        TargetMesh->SetSimulatePhysics(false);
    }*/
}

void UDropItemMover::UpdateMovement(float DeltaTime)
{

    if (!bOnGround)
    {
        FVector newOrigin = CurrentMatrix.GetOrigin();
        const float gravity = -980.f;//仮重力
        Velocity.Z += gravity * DeltaTime;

        newOrigin += Velocity * DeltaTime;

        // 地面判定
        FHitResult hitResult;
        FVector traceStart = newOrigin + FVector(0, 0, 100);
        FVector traceEnd = newOrigin - FVector(0, 0, 100);

        FCollisionQueryParams Params;
        Params.AddIgnoredActor(GetOwner());

        bool bHit = GetWorld()->LineTraceSingleByChannel(
            hitResult, traceStart, traceEnd, ECC_WorldStatic, Params
        );

        if (bHit)
        {
            newOrigin = hitResult.Location;
            bOnGround = true;
            Velocity = FVector::ZeroVector;

            GroundLocation = newOrigin;
            FloatingTime = 0.f;
            //TargetMesh->SetSimulatePhysics(false);
            UKismetSystemLibrary::PrintString(this, TEXT("DropItemMover: Landed"), true, true, FColor::Green, 2.f);
        }

        CurrentMatrix.SetOrigin(newOrigin);
    }
    else
    {
        FloatingTime += DeltaTime;

        // 2. sin波を使って上下のオフセットを計算
        float zOffset = FloatingLength * FMath::Sin(FloatingSpeed * FloatingTime);
        FVector newOrigin = GroundLocation + FVector(0.f, 0.f, zOffset);
        
        // --- ここまで変更 ---

        // Z軸回転
        float yawSpeed = 60.f;
        float yawRadians = FMath::DegreesToRadians(yawSpeed * DeltaTime);

        FQuat deltaQuat(FVector::UpVector, yawRadians);
        FMatrix deltaMatrix = FRotationMatrix::Make(deltaQuat.Rotator());

        CurrentMatrix = CurrentMatrix * deltaMatrix;

        // ピッチとロールを固定
        FRotator Rot = CurrentMatrix.Rotator();
        Rot.Pitch = 0.f;
        Rot.Roll = 0.f;

        CurrentMatrix = FRotationMatrix(Rot);
        CurrentMatrix.SetOrigin(newOrigin);
    }

    // 反映
    if (AActor* Owner = GetOwner())
    {
        Owner->SetActorLocationAndRotation(CurrentMatrix.GetOrigin(), CurrentMatrix.Rotator());
    }
}

void UDropItemMover::HandlePickedUp(ACharacterBase* PickUpCharacter)
{
    StopMovement();
}

void UDropItemMover::HandleDropped()
{
    ResetMovement();
}
