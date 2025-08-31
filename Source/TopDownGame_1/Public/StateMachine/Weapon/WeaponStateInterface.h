#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponStateInterface.generated.h"

// このクラスは Blueprint に対応するインターフェース用のベース
UINTERFACE(Blueprintable)
class UWeaponStateInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * ステートのインターフェース本体
 */
class TOPDOWNGAME_1_API IWeaponStateInterface
{
    GENERATED_BODY()

public:

    //ステートに入ったとき
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponState")
    void OnEnter(AWeaponBase* Weapon);

    //ステートから抜けるとき
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponState")
    void OnExit();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponState")
    void Fire();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponState")
    void EndFire();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponState")
    void Reload();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponState")
    void Drop();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponState")
    void Tick(float DeltaTime);
};
