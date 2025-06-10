// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "WeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TOPDOWNGAME_1_API UWeaponDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Info")
    FText WeaponName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
    float AttackPower = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
    float FireRate = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
    int32 MagazineSize = 30;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
    float ReloadTime = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
    UTexture2D* WeaponIcon;
};