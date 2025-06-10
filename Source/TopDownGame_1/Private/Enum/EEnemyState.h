// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EEnemyState.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
    // —ñ‹“q‚ğ‹Lq
    Idle = 0    UMETA(DisplayName = "Idle"),
    Search = 1  UMETA(DisplayName = "Search"),
    Find = 2    UMETA(DisplayName = "Find"),
    Death = 3   UMETA(DisplayName = "Death"),
};