// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter_Wraith.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ProjectMacros.h"
#include "WeaponBase.h"
#include "EquipmentComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

float const DefaultMeshRotate = -90.0f;

APlayerCharacter_Wraith::APlayerCharacter_Wraith() : Super()
{

    GetCharacterMovement()->bOrientRotationToMovement = true;
}

void APlayerCharacter_Wraith::BeginPlay()
{
    Super::BeginPlay();
    //Wraithに元からついてる武器隠す
    GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
    //default装備
    /*if(DefaultHoldWeapon)
    EquipmentComponent->Equip(GetWorld()->SpawnActor<AWeaponBase>(DefaultHoldWeapon),GetMesh());*/
    PlayerController = Cast<APlayerController>(GetController());
}

void APlayerCharacter_Wraith::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

    if (!PlayerController) return;

    FVector WorldLocation;  // マウス位置のワールド座標
    FVector WorldDirection; // マウス方向ベクトル

    // マウスのスクリーン座標を取得
    float ScreenX, ScreenY;
    PlayerController->GetMousePosition(ScreenX, ScreenY);

    // スクリーン座標をワールド座標と方向ベクトルに変換
    if (PlayerController->DeprojectScreenPositionToWorld(ScreenX, ScreenY, WorldLocation, WorldDirection))
    {
        // LineTraceを実行
        FVector Start = WorldLocation;
        FVector End = Start + (WorldDirection * 5000.0f); // 5000ユニット先までトレース

        FHitResult HitResult;
        FCollisionQueryParams Params; 
        Params.AddIgnoredActor(this); // 自分自身を無視

        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

        if (bHit)
        {
            FVector hitLocation = HitResult.Location;
            // デバッグライン描画
            //DrawDebugLine(GetWorld(), Start, hitLocation, FColor::Red, false, 2.0f, 0, 2.0f);
            FVector currentLocation = GetActorLocation();
            FVector lookStart = FVector(currentLocation.X,currentLocation.Y,0.0f);
            FVector lookTarget = FVector(hitLocation.X, hitLocation.Y, 0.0f);
            FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(lookStart,lookTarget);
            MouseAngle = ANGLE_DIFFERENCE(GetActorRotation().Yaw,TargetRotation.Yaw );
            if (SkeletalMesh) {
                //目的の回転値＋Wraithのデフォルト回転
                TargetRotation.Yaw += DefaultMeshRotate;
                SkeletalMesh->SetWorldRotation(TargetRotation);
            }
            //SetActorRotation(TargetRotation);
            //UKismetSystemLibrary::PrintString(this,  MouseAngle.Pitch.ToString(), true, true, FColor::Cyan, 2.f, TEXT("None"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No hit detected"));
        }
    }

    
}

void APlayerCharacter_Wraith::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    EquipmentComponent->DiscardWeapon();
}

void APlayerCharacter_Wraith::Fire()
{
    //HoldWeapon->Fire();
    EquipmentComponent->Fire();
}

void APlayerCharacter_Wraith::EndFire()
{
    //HoldWeapon->EndFire();
    EquipmentComponent->EndFire();
}