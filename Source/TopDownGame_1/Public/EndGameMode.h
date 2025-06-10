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
	// �\������E�B�W�F�b�g�N���X���w��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> EndScreenWidgetClass;

private:
	// �쐬�����E�B�W�F�b�g�̎Q��
	UPROPERTY()
	class UUserWidget* EndScreenWidget;
};
