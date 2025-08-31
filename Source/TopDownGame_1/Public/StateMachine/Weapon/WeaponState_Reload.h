#pragma once
#include "CoreMinimal.h"
#include "StateMachine/Weapon/WeaponStateBase.h"
#include "WeaponState_Reload.generated.h"

UCLASS()
class TOPDOWNGAME_1_API UWeaponState_Reload : public UWeaponStateBase
{
    GENERATED_BODY()

protected:
    virtual void OnEnter_Implementation(AWeaponBase* Weapon) override;
    virtual void Fire_Implementation() override;
    virtual void EndFire_Implementation() override;
    virtual void Drop_Implementation() override;
    // リロード完了時に呼ばれる関数
    UFUNCTION()
    void OnReloadFinished();
    TSubclassOf<UWeaponStateBase> StandbyState;
private:
    FTimerHandle ReloadTimerHandle;
};