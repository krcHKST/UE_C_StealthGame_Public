// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TitleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNGAME_1_API ATitleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
protected:
	// �\������widget�̃N���X�w��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> TitleScreenWidgetClass;

private:
	UPROPERTY()//�����widget�̎Q��
	class UUserWidget* TitleScreenWidget;
	
};
