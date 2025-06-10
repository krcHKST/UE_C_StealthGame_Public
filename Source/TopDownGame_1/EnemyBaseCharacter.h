// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Private/Enum/EEnemyState.h"
#include "EnemyBaseCharacter.generated.h"

UCLASS()
class TOPDOWNGAME_1_API AEnemyBaseCharacter : public ACharacterBase
{
	GENERATED_BODY()

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

protected:
public:
	AEnemyBaseCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	/*UPROPERTY(VisibleAnywhere, Category = "AI")*/
	//class UPawnSensingComponent* PawnSensingComp;
	UPROPERTY(EditAnywhere,Category = "AI")
	class AEnemyRoamingPoint* RoamingPointActor;
	UPROPERTY(EditAnywhere, Category = "Perceptions")
	float SightRadius;
	UPROPERTY(EditAnywhere,Category = "Perceptions")
	float LoseSightRadius;
	UPROPERTY(EditAnywhere, Category = "Perceptions")
	float AngleDegrees;
	UPROPERTY()
	float isFind = false;
	UPROPERTY(VisibleAnywhere)
	EEnemyState EnemyState = EEnemyState::Idle;
	UFUNCTION()
	void OnSeePlayer(ACharacter* character);
	UFUNCTION()
	class AEnemyRoamingPoint* GetRoamingPointActor();
};
