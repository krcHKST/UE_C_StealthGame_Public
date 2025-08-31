#pragma once

#include "CoreMinimal.h"
#include "WeaponStats.generated.h"

USTRUCT(BlueprintType)
struct FWeaponStats
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText WeaponName = FText::FromString(TEXT("None"));
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float AttackPower = 10.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 OneShotNum = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float FireRate = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MagazineSize = 30;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ReloadTime = 2.0f;
};
