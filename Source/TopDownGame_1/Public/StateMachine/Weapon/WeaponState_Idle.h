#pragma once

#include "CoreMinimal.h"
#include "StateMachine/Weapon/WeaponStateBase.h"
#include "WeaponState_Idle.generated.h"

UCLASS()
class TOPDOWNGAME_1_API UWeaponState_Idle : public UWeaponStateBase
{
    GENERATED_BODY()

protected:
    // IWeaponStateInterfaceの関数をオーバーライド
    virtual void Fire_Implementation() override;
    virtual void Reload_Implementation() override;
    virtual void Tick_Implementation(float DeltaTime) override;
};