#include "StateMachine/Weapon/WeaponStateMachineComponent.h"
#include "StateMachine/Weapon//WeaponState_Idle.h"
#include "StateMachine/Weapon//WeaponState_Fire.h"
#include "StateMachine/Weapon//WeaponState_Reload.h"
#include "WeaponBase.h"

UWeaponStateMachineComponent::UWeaponStateMachineComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    IdleStateClass = UWeaponState_Idle::StaticClass();
    FireStateClass = UWeaponState_Fire::StaticClass();
    ReloadStateClass = UWeaponState_Reload::StaticClass();
}

void UWeaponStateMachineComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerWeapon = Cast<AWeaponBase>(GetOwner());
    if (!OwnerWeapon)
    {
        UE_LOG(LogTemp, Error, TEXT("WeaponStateMachineComponent:'%s' is not WeaponBase"), *GetOwner()->GetName());
        SetComponentTickEnabled(false);
        return;
    }
    ChangeStateByClass(IdleStateClass);
}

void UWeaponStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (CurrentState)
    {
        CurrentState->Execute_Tick(CurrentState.GetObject(), DeltaTime);
    }
}

void UWeaponStateMachineComponent::Fire()
{
    if (CurrentState)
    {
        CurrentState->Execute_Fire(CurrentState.GetObject());
    }
}

void UWeaponStateMachineComponent::EndFire()
{
    if (CurrentState)
    {
        CurrentState->Execute_EndFire(CurrentState.GetObject());
    }
}

void UWeaponStateMachineComponent::Reload()
{
    if (CurrentState)
    {
        CurrentState->Execute_Reload(CurrentState.GetObject());
    }
}

void UWeaponStateMachineComponent::ChangeState(TScriptInterface<IWeaponStateInterface> NewState)
{
    if (CurrentState)
    {
        BeforeState = CurrentState;
        CurrentState->Execute_OnExit(CurrentState.GetObject());
    }

    CurrentState = NewState;

    if (CurrentState)
    {
        CurrentState->Execute_OnEnter(CurrentState.GetObject(), OwnerWeapon);
    }
}

void UWeaponStateMachineComponent::ChangeStateByClass(TSubclassOf<UObject> NewStateClass)
{
    if (!NewStateClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("WeaponStateMachineComponent: ChangeStateByClass is nullptr"));
        return;
    }

    // 現在と同じステートなら何もしない
    if (CurrentState && CurrentState.GetObject()->GetClass() == NewStateClass)
    {
        return;
    }

    TScriptInterface<IWeaponStateInterface> newState;

    // 既存のインスタンスを探す
    if (TScriptInterface<IWeaponStateInterface>* foundState = StateInstances.Find(NewStateClass))
    {
        newState = *foundState;
    }
    else
    {
        // 新しくステートオブジェクトを生成
        UObject* newStateObject = NewObject<UObject>(this, NewStateClass);
        TScriptInterface<IWeaponStateInterface> newStateInterface = newStateObject;

        if (newStateInterface)
        {
            // 生成したインスタンスをマップに保存して再利用可能にする
            StateInstances.Add(NewStateClass, newStateInterface);
            newState = newStateInterface;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create state: Class '%s' does not implement IWeaponStateInterface."), *NewStateClass->GetName());
            return;
        }
    }
    // 実際のステート切り替え処理
    ChangeState(newState);
}