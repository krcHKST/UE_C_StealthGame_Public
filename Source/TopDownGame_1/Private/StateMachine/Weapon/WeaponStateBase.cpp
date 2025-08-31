#include "StateMachine/Weapon/WeaponStateBase.h"
#include "WeaponBase.h" 
#include "StateMachine/Weapon/WeaponStateMachineComponent.h"

UWeaponStateBase::UWeaponStateBase()
{
}

void UWeaponStateBase::OnEnter_Implementation(AWeaponBase* Weapon)
{
    // ステートに入った時ポインタをキャッシュ
    OwnerWeapon = Weapon;
    if (OwnerWeapon)
    {
        StateMachineComponent = OwnerWeapon->FindComponentByClass<UWeaponStateMachineComponent>();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("WeaponState:ownerWeapon not found"));
    }
}

void UWeaponStateBase::OnExit_Implementation()
{
    //参照をクリア
    OwnerWeapon = nullptr;
    StateMachineComponent = nullptr;
}

// デフォルトは空実装
void UWeaponStateBase::Fire_Implementation() {}
void UWeaponStateBase::EndFire_Implementation() {}
void UWeaponStateBase::Reload_Implementation() {}
void UWeaponStateBase::Drop_Implementation() {}
void UWeaponStateBase::Tick_Implementation(float DeltaTime) {}