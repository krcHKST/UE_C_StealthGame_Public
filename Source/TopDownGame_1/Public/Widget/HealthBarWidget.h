// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UHealthComponent;
class UProgressBar;
/**
 * 
 */
UCLASS()
class TOPDOWNGAME_1_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 敵キャラクターから呼ばれ、監視対象のHealthComponentを設定する
	void SetHealthComponent(UHealthComponent* NewHealthComp);
protected:
	// BlueprintのWidgetとC++のプロパティを接続する
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HP_ProgressBar;

	UFUNCTION()
	void HandleOnHealthChanged(float NewHealth, float MaxHealth);

	// このWidgetが監視しているHealthComponent
	UPROPERTY()
	TWeakObjectPtr<UHealthComponent> HealthComponent;

	// Widgetが破棄される際に呼ばれる
	virtual void NativeDestruct() override;
	
};
