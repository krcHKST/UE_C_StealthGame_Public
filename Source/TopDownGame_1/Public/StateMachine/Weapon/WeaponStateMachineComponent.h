#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachine/Weapon/WeaponStateInterface.h"
#include "WeaponStateMachineComponent.generated.h"

class AWeaponBase;
class UWeaponStateBase;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPDOWNGAME_1_API UWeaponStateMachineComponent : public UActorComponent
{
    GENERATED_BODY()
protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UWeaponStateBase> IdleStateClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UWeaponStateBase> FireStateClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UWeaponStateBase> ReloadStateClass;
public:
    UWeaponStateMachineComponent();
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
    void Fire();
    UFUNCTION(BlueprintCallable)
    void EndFire();
    UFUNCTION(BlueprintCallable)
    void Reload();
    UFUNCTION(BlueprintCallable)
    void ChangeState(TScriptInterface<IWeaponStateInterface> NewState);
    UFUNCTION(BlueprintCallable)
    void ChangeStateByClass(TSubclassOf<UObject> NewStateClass);
    UFUNCTION(BlueprintPure)
    TScriptInterface<IWeaponStateInterface> GetCurrentState() const { return CurrentState; }
    TScriptInterface<IWeaponStateInterface> GetBeforeState() const { return BeforeState; }
    //get,set
    TSubclassOf<UWeaponStateBase> GetIdleStateClass() const { return IdleStateClass; }
    TSubclassOf<UWeaponStateBase> GetFireStateClass() const { return FireStateClass; }
    TSubclassOf<UWeaponStateBase> GetReloadStateClass() const { return ReloadStateClass; }
private:
    TScriptInterface<IWeaponStateInterface> CurrentState;//参考　https://dev.epicgames.com/documentation/ja-jp/unreal-engine/interfaces-in-unreal-engine
    TScriptInterface<IWeaponStateInterface> BeforeState;
    UPROPERTY()
    AWeaponBase* OwnerWeapon;
    UPROPERTY()
    TMap<TSubclassOf<UObject>, TScriptInterface<IWeaponStateInterface>> StateInstances;

};
