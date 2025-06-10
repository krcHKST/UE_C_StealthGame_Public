// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	//ï‡çsë¨ìx 
	UPROPERTY(BlueprintReadWrite) 
		float WalkSpeed = 0.0f;
	UPROPERTY(BlueprintReadWrite)
	bool IsFall;
public: 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
