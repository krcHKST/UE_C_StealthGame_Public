#include "StateMachine/Weapon/WeaponState_Fire.h"
#include "StateMachine/Weapon/WeaponState_Idle.h"
#include "StateMachine/Weapon/WeaponState_Reload.h"
#include "StateMachine/Weapon/WeaponStateMachineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponBase.h"

void UWeaponState_Fire::OnEnter_Implementation(AWeaponBase* Weapon)
{
    Super::OnEnter_Implementation(Weapon);
}

void UWeaponState_Fire::Tick_Implementation(float DeltaTime)
{
    Super::Tick_Implementation(DeltaTime);
    if (!IsValid(this) || !IsValid(OwnerWeapon)) return;
    //発射音
    USoundBase* const sound = OwnerWeapon->FireSound;
    const FVector soundLocation = OwnerWeapon->GetActorLocation();
    OwnerWeapon->UpdateFireTimer(DeltaTime);
    if (OwnerWeapon->ReadyToFire())
    {
        OwnerWeapon->SpawnProjectile();

        if (sound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound, soundLocation);
        }
    }
}

void UWeaponState_Fire::EndFire_Implementation()
{
    TSubclassOf<UWeaponStateBase> idleState = StateMachineComponent->GetIdleStateClass();
    // ステートマシンが存在しIdleStateClassが設定されていれば遷移
    if (StateMachineComponent && idleState)
    {
        StateMachineComponent->ChangeStateByClass(idleState);
    }
}

void UWeaponState_Fire::Reload_Implementation()
{
    if (!OwnerWeapon) return;
    TSubclassOf<UWeaponStateBase> reloadState = StateMachineComponent->GetReloadStateClass();
    // ステートマシンが存在しReloadStateClassが設定されていれば遷移
    if (StateMachineComponent && reloadState)
    {
        OwnerWeapon->GetCurrentAmmo();
        StateMachineComponent->ChangeStateByClass(reloadState);
    }
}

void UWeaponState_Fire::Drop_Implementation()
{
    TSubclassOf<UWeaponStateBase> idleState = StateMachineComponent->GetIdleStateClass();
    // ステートマシンが存在しIdleStateClassが設定されていれば遷移
    if (StateMachineComponent && idleState)
    {
        StateMachineComponent->ChangeStateByClass(idleState);
    }
}
