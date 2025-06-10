// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Camera/CameraComponent.h"
#include "TopDownGame_1/TopDownGame_1Character.h"
#include "CameraPixelatedCapture.generated.h"

UCLASS()
class ACameraPixelatedCapture : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
	USceneCaptureComponent2D* sceneCapture;

	ATopDownGame_1Character* attachCharacter;

	// Sets default values for this actor's properties
	ACameraPixelatedCapture();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
