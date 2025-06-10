#include "EndGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void AEndGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (EndScreenWidgetClass)
	{
		EndScreenWidget = CreateWidget<UUserWidget>(GetWorld(), EndScreenWidgetClass);
		if (EndScreenWidget)
		{
			EndScreenWidget->AddToViewport();

			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				// UI���͐�p & �}�E�X�J�[�\����\��
				PC->bShowMouseCursor = true;
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(EndScreenWidget->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PC->SetInputMode(InputMode);
			}
		}
	}
}
