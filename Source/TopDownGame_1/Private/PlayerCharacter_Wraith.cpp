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
    //Wraith�Ɍ�������Ă镐��B��
    GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
    //default����
    /*if(DefaultHoldWeapon)
    EquipmentComponent->Equip(GetWorld()->SpawnActor<AWeaponBase>(DefaultHoldWeapon),GetMesh());*/
    PlayerController = Cast<APlayerController>(GetController());
}

void APlayerCharacter_Wraith::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

    if (!PlayerController) return;

    FVector WorldLocation;  // �}�E�X�ʒu�̃��[���h���W
    FVector WorldDirection; // �}�E�X�����x�N�g��

    // �}�E�X�̃X�N���[�����W���擾
    float ScreenX, ScreenY;
    PlayerController->GetMousePosition(ScreenX, ScreenY);

    // �X�N���[�����W�����[���h���W�ƕ����x�N�g���ɕϊ�
    if (PlayerController->DeprojectScreenPositionToWorld(ScreenX, ScreenY, WorldLocation, WorldDirection))
    {
        // LineTrace�����s
        FVector Start = WorldLocation;
        FVector End = Start + (WorldDirection * 5000.0f); // 5000���j�b�g��܂Ńg���[�X

        FHitResult HitResult;
        FCollisionQueryParams Params; 
        Params.AddIgnoredActor(this); // �������g�𖳎�

        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

        if (bHit)
        {
            FVector hitLocation = HitResult.Location;
            // �f�o�b�O���C���`��
            //DrawDebugLine(GetWorld(), Start, hitLocation, FColor::Red, false, 2.0f, 0, 2.0f);
            FVector currentLocation = GetActorLocation();
            FVector lookStart = FVector(currentLocation.X,currentLocation.Y,0.0f);
            FVector lookTarget = FVector(hitLocation.X, hitLocation.Y, 0.0f);
            FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(lookStart,lookTarget);
            MouseAngle = ANGLE_DIFFERENCE(GetActorRotation().Yaw,TargetRotation.Yaw );
            if (SkeletalMesh) {
                //�ړI�̉�]�l�{Wraith�̃f�t�H���g��]
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