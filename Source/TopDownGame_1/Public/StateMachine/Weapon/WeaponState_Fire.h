#pragma once

#include "CoreMinimal.h"
#include "StateMachine/Weapon/WeaponStateBase.h"
#include "WeaponState_Fire.generated.h"

UCLASS()
class TOPDOWNGAME_1_API UWeaponState_Fire : public UWeaponStateBase
{
    GENERATED_BODY()

protected:
    virtual void OnEnter_Implementation(AWeaponBase* Weapon) override;
    virtual void Tick_Implementation(float DeltaTime) override;
    virtual void EndFire_Implementation() override;
    virtual void Reload_Implementation() override;
    virtual void Drop_Implementation() override;
};