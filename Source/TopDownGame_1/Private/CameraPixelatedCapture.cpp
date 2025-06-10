// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPixelatedCapture.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACameraPixelatedCapture::ACameraPixelatedCapture()
{
	sceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraPixelatedCapture::BeginPlay()
{
	Super::BeginPlay();
	attachCharacter = Cast<ATopDownGame_1Character>(UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0));
	UCameraComponent* camera = attachCharacter->GetTopDownCameraComponent();
	float fov = camera->FieldOfView;
	sceneCapture->AttachToComponent((USceneComponent*)camera, FAttachmentTransformRules::SnapToTargetIncludingScale);
	sceneCapture->FOVAngle = fov;
}

// Called every frame
void ACameraPixelatedCapture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	sceneCapture->CaptureScene();
}

