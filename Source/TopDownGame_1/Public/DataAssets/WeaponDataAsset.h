// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "WeaponStats.h"
#include "WeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TOPDOWNGAME_1_API UWeaponDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
    FWeaponStats WeaponStats;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Info")
    UTexture2D* WeaponIcon;
};