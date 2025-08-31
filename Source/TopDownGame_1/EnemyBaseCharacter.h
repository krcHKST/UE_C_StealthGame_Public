// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Public/Enum/Enemy/EEnemyState.h"
#include "Interface/DamageModifierInterface.h"
#include "EnemyBaseCharacter.generated.h"

class ARoomBase;
class UWidgetComponent;

UCLASS()
class TOPDOWNGAME_1_API AEnemyBaseCharacter : public ACharacterBase,public IDamageModifierInterface
{
	GENERATED_BODY()

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

protected:
	UPROPERTY(EditAnywhere)
	ARoomBase* ParentRoom;//スポーン時、もしくはエディタ上で配置時に設定
	UPROPERTY()
	int32 RoamingID;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* HealthWidgetComponent;
	UPROPERTY(EditAnywhere)
	FName PlayerKeyName;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	bool bRandomWeapon;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<TSubclassOf<AWeaponBase>> RandomWeaponCandidates;
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
	UPROPERTY(VisibleAnywhere)
	EEnemyState EnemyState = EEnemyState::Idle;
	UFUNCTION()
	void OnSeePlayer(ACharacter* character);
	//インターフェースのダメージ計算関数
	//状態などに応じたダメージ計算後の値をHealthComponentデリゲート側に渡すため
	virtual float ApplyDamageModification_Implementation(float BaseDamage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//get,set

	UFUNCTION()
	class AEnemyRoamingPoint* GetRoamingPointActor();
	UFUNCTION()
	EEnemyState GetBBEnemyState();
	UFUNCTION()
	void SetParentRoom(ARoomBase* room) { ParentRoom = room; };
	UFUNCTION()
	ARoomBase* GetParentRoom() { return ParentRoom; };
	UFUNCTION(BlueprintCallable)
	int32 GetRoamingID(){ return RoamingID; };
	void SetRoamingID(int32 setNum) { RoamingID = setNum; };
};
