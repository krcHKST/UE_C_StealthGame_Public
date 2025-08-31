#include "StateMachine/Weapon/WeaponState_Idle.h"
#include "StateMachine/Weapon/WeaponState_Fire.h"
#include "StateMachine/Weapon/WeaponState_Reload.h"
#include "StateMachine/Weapon/WeaponStateMachineComponent.h"
#include "WeaponBase.h"

void UWeaponState_Idle::Fire_Implementation()
{
    TSubclassOf<UWeaponStateBase> fireState = StateMachineComponent->GetFireStateClass();
    // ステートマシンが存在しFireStateClassが設定されていれば遷移
    if (StateMachineComponent && fireState)
    {
        StateMachineComponent->ChangeStateByClass(fireState);
    }
}

void UWeaponState_Idle::Reload_Implementation()
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

void UWeaponState_Idle::Tick_Implementation(float DeltaTime)
{
    if (!OwnerWeapon) return;
    OwnerWeapon->UpdateFireTimer(DeltaTime);

}
