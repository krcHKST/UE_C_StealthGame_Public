// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "HealthComponent.h" 

void UHealthBarWidget::SetHealthComponent(UHealthComponent* NewHealthComp)
{
    if (!NewHealthComp)
    {
        return;
    }

    // 弱参照ポインタでHealthComponentを保持
    HealthComponent = NewHealthComp;

    // HealthComponentのOnHealthChangedデリゲートにバインド
    NewHealthComp->OnHealthChanged.AddDynamic(this, &UHealthBarWidget::HandleOnHealthChanged);

    // 初期値を設定
    HandleOnHealthChanged(NewHealthComp->GetHealth(), NewHealthComp->GetMaxHealth());
}

void UHealthBarWidget::HandleOnHealthChanged(float NewHealth, float MaxHealth)
{
    if (HP_ProgressBar)
    {
        const float Percent = (MaxHealth > 0.0f) ? (NewHealth / MaxHealth) : 0.0f;
        HP_ProgressBar->SetPercent(Percent);
    }
}

void UHealthBarWidget::NativeDestruct()
{
    // Widgetが破棄される前に、バインドを解除してメモリリークやクラッシュを防ぐ
    if (HealthComponent.IsValid())
    {
        HealthComponent->OnHealthChanged.RemoveDynamic(this, &UHealthBarWidget::HandleOnHealthChanged);
    }

    Super::NativeDestruct();
}