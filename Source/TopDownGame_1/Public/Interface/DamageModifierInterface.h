// DamageModifierInterface.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageModifierInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UDamageModifierInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *ダメージ計算の挙動を変更するためのインターフェース
 */
class TOPDOWNGAME_1_API IDamageModifierInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	float ApplyDamageModification(float BaseDamage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
};