#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StateMachine/Weapon/WeaponStateInterface.h"
#include "WeaponStateBase.generated.h"

class AWeaponBase;
class UWeaponStateMachineComponent;

/**
 * 全ての武器ステートの基底クラス
 */
UCLASS(Blueprintable, BlueprintType)
class TOPDOWNGAME_1_API UWeaponStateBase : public UObject, public IWeaponStateInterface
{
    GENERATED_BODY()

public:
    UWeaponStateBase();
    virtual void OnEnter_Implementation(AWeaponBase* Weapon) override;
    virtual void OnExit_Implementation() override;
    virtual void Fire_Implementation() override;
    virtual void EndFire_Implementation() override;
    virtual void Reload_Implementation() override;
    virtual void Drop_Implementation() override;
    virtual void Tick_Implementation(float DeltaTime) override;

protected:
    // このステートが操作する武器本体
    UPROPERTY(BlueprintReadOnly, Category = "State")
    AWeaponBase* OwnerWeapon;

    // このステートを管理するステートマシン
    UPROPERTY(BlueprintReadOnly, Category = "State")
    UWeaponStateMachineComponent* StateMachineComponent;
};