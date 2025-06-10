// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNGAME_1_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
public:
	AMainGameMode();
	virtual void PawnKilled(APawn* pawnKilled);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> PlayerWidget;

	UPROPERTY()
	UUserWidget* CurrentWidget;
};
