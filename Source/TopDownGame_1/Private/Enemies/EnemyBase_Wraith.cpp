// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyBase_Wraith.h"
#include "WeaponBase.h"
#include "EquipmentComponent.h"
#include "Components/CapsuleComponent.h"

AEnemyBase_Wraith::AEnemyBase_Wraith() : Super()
{
}

void AEnemyBase_Wraith::BeginPlay()
{
    Super::BeginPlay();
    //Wraith�Ɍ�������Ă镐��B��
    GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
    //��������Z�b�g
    /*if(DefaultHoldWeapon)EquipmentComponent->Equip(GetWorld()->SpawnActor<AWeaponBase>(DefaultHoldWeapon),GetMesh());*/
}

void AEnemyBase_Wraith::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AEnemyBase_Wraith::DeathReaction()
{
	Super::DeathReaction();
	USkeletalMeshComponent* meshComp = GetMesh();
	if (!meshComp) return;

	// ���O�h�[����
	meshComp->SetCollisionProfileName(TEXT("Ragdoll"));
	meshComp->SetSimulatePhysics(true);
	// ���[�g�̃R���g���[������~
	/*meshComp->SetAllBodiesSimulatePhysics(true);
	meshComp->WakeAllRigidBodies();*///���׃`�F�b�N�̂��߂Ƃ肠�����R�����g�A�E�g
	meshComp->bBlendPhysics = true;

	// �R���g���[���[�𖳌���
	DetachFromControllerPendingDestroy();

	// �J�v�Z���𖳌���
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}