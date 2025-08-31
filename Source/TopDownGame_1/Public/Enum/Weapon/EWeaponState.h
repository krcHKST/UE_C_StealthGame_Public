
#pragma once

#include "CoreMinimal.h"
#include "EWeaponState.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
    // 列挙子を記述
    Idle = 0    UMETA(DisplayName = "Idle"),
    Fire = 1  UMETA(DisplayName = "Fire"),
    Reload = 2    UMETA(DisplayName = "Reload"),
};