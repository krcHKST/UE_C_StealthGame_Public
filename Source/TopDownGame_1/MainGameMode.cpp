// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "TopDownGame_1PlayerController.h"
#include "TopDownGame_1Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

AMainGameMode::AMainGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATopDownGame_1PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	// プレイヤーキャラクター取得
	ATopDownGame_1Character* PlayerCharacter = Cast<ATopDownGame_1Character>(UGameplayStatics::GetPlayerCharacter(this, 0));

	// UI作成 & ビューポートに追加
	if (PlayerWidget)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerWidget);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
	// プレイヤーコントローラ取得
	APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (pc)
	{
		// 入力モード設定
		FInputModeGameAndUI InputMode;
		//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // マウスを画面内に固定しない

		pc->SetInputMode(InputMode);
	}
}


void AMainGameMode::PawnKilled(APawn* pawnKilled)
{
	// 死亡したのがプレイヤーかを確認
	APlayerController* PlayerController = Cast<APlayerController>(pawnKilled->GetController());
	if (PlayerController != nullptr)
	{
		//プレイヤーの入力を無効化
		PlayerController->DisableInput(PlayerController);
		//5秒後にRestartLevelを呼び出すタイマーを設定
		GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &AMainGameMode::RestartLevel, 5.0f, false);
	}
}

void AMainGameMode::RestartLevel()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(World);
		//現在のレベルを開く
		UGameplayStatics::OpenLevel(World, FName(*CurrentLevelName));
	}
}