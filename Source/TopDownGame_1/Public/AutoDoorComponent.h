#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AutoDoorComponent.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class ACharacterBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOPDOWNGAME_1_API UAutoDoorComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UAutoDoorComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, Category = "Door Settings")
	bool bCanOpen = true;

	UPROPERTY(EditAnywhere, Category = "Door Settings")
	FVector OpenOffset = FVector(0.f, 150.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Door Settings")
	float OpenSpeed = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* LeftDoorMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* RightDoorMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerVolume = nullptr;

private:
	//èâä˙à íu
	FVector LeftDoorClosedPosition;
	FVector RightDoorClosedPosition;

	int32 OverlapCharacterCount = 0;
	bool bShouldOpen = false;

	UFUNCTION()
	void OnTriggerBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
