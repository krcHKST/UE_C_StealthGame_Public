#pragma once
#include "CoreMinimal.h"
#include "ERoomType.generated.h"

UENUM(BlueprintType)
enum class ERoomType : uint8
{
    Normal = 0 UMETA(DisplayName = "Normal"),
    Start = 1 UMETA(DisplayName = "Start"),
    End = 2 UMETA(DisplayName = "End"),
};
