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
    //Wraithに元からついてる武器隠す
    GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
    //初期武器セット
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

	// ラグドール化
	meshComp->SetCollisionProfileName(TEXT("Ragdoll"));
	meshComp->SetSimulatePhysics(true);
	// ルートのコントロールも停止
	/*meshComp->SetAllBodiesSimulatePhysics(true);
	meshComp->WakeAllRigidBodies();*///負荷チェックのためとりあえずコメントアウト
	meshComp->bBlendPhysics = true;

	// コントローラーを無効化
	DetachFromControllerPendingDestroy();

	// カプセルを無効化
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}