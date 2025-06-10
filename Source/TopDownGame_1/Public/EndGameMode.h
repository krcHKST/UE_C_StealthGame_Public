// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndGameMode.generated.h"

/**
 *
 */
UCLASS()
class TOPDOWNGAME_1_API AEndGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	// 表示するウィジェットクラスを指定
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> EndScreenWidgetClass;

private:
	// 作成したウィジェットの参照
	UPROPERTY()
	class UUserWidget* EndScreenWidget;
};
