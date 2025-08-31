#include "StateMachine/Weapon/WeaponState_Reload.h"
#include "StateMachine/Weapon/WeaponStateMachineComponent.h"
#include "WeaponBase.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void UWeaponState_Reload::OnEnter_Implementation(AWeaponBase* Weapon)
{
    Super::OnEnter_Implementation(Weapon);
    
    if (OwnerWeapon)
    {
        //リロ開始音
        if (OwnerWeapon->ReloadStartSound)
        {
            UGameplayStatics::PlaySoundAtLocation(OwnerWeapon, OwnerWeapon->ReloadStartSound, OwnerWeapon->GetActorLocation());
        }

        //やるとしたらここでリロードアニメーションの再生

        //指定時間後に OnReloadFinished を呼び出すタイマーを設定
        GetWorld()->GetTimerManager().SetTimer(
            ReloadTimerHandle,
            this,
            &UWeaponState_Reload::OnReloadFinished,
            OwnerWeapon->GetWeaponStats().ReloadTime,
            false
        );
    }
}

void UWeaponState_Reload::Fire_Implementation()
{
    StandbyState = StateMachineComponent->GetFireStateClass();
}

void UWeaponState_Reload::EndFire_Implementation()
{
    StandbyState = StateMachineComponent->GetIdleStateClass();
}

void UWeaponState_Reload::Drop_Implementation()
{
    StandbyState = StateMachineComponent->GetIdleStateClass();
}

void UWeaponState_Reload::OnReloadFinished()
{
    if (!OwnerWeapon)return;
    OwnerWeapon->FullAmmo();

    //リロ完了音
    if (OwnerWeapon->ReloadEndSound)
    {
        UGameplayStatics::PlaySoundAtLocation(OwnerWeapon, OwnerWeapon->ReloadEndSound, OwnerWeapon->GetActorLocation());
    }

    // ステートマシンが存在しBeforeClassが存在すれば遷移
    if (StateMachineComponent)
    {
        if(StandbyState) StateMachineComponent->ChangeStateByClass(StandbyState);
        //BeforeがなければIdleに戻る
        else StateMachineComponent->ChangeStateByClass(StateMachineComponent->GetIdleStateClass());
    }
}