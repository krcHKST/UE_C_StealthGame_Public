#include "TitleGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (TitleScreenWidgetClass)
	{
		TitleScreenWidget = CreateWidget<UUserWidget>(GetWorld(), TitleScreenWidgetClass);
		if (TitleScreenWidget)
		{
			TitleScreenWidget->AddToViewport();

			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				// UI��p�̓��̓��[�h�ɐݒ� & �}�E�X�J�[�\���\��
				PC->bShowMouseCursor = true;
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(TitleScreenWidget->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PC->SetInputMode(InputMode);
			}
		}
	}
}
