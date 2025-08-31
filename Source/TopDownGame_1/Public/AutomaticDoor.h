#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AutomaticDoor.generated.h"

class UBoxComponent;
class ACharacterBase;

UCLASS()
class TOPDOWNGAME_1_API AAutomaticDoor : public AActor
{
	GENERATED_BODY()

public:
	AAutomaticDoor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	bool bCanOpen = true;
protected:
	// 左ドアのメッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* LeftDoorMesh;

	// 右ドアのメッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* RightDoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerVolume;

	// 片方のドアの移動オフセット
	UPROPERTY(EditAnywhere, Category = "Door Settings")
	FVector OpenOffset;

	// ドアの開閉速度
	UPROPERTY(EditAnywhere, Category = "Door Settings")
	float OpenSpeed = 200.f;

private:
	// 左右のドアの閉じた位置
	FVector LeftDoorClosedPosition;
	FVector RightDoorClosedPosition;

	int32 OverlapCharacterCount = 0;
	bool bShouldOpen = false;

	UFUNCTION()
	void OnTriggerBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};