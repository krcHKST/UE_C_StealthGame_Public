// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enum/ERoomType.h"
#include "RoomBase.generated.h"

class UEnemyRoamingPointComponent;
class USpawnPointComponent;
class UChildActorComponent;
class AAutomaticDoor;

// ドアの方向を示すためのEnum
UENUM(BlueprintType)
enum class EDoorDirection : uint8
{
    Forward,//+X
    Backward,//-X
    Right,   //+Y
    Left     //-Y
};


UCLASS()
class TOPDOWNGAME_1_API ARoomBase : public AActor
{
    GENERATED_BODY()

public:
    ARoomBase();

    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintPure)
    ERoomType GetRoomType() { return RoomType; };

    //この部屋がどの方向に接続できるかを示すフラグ
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room Settings")
    bool bCanConnectForward = true; //+X方向
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room Settings")
    bool bCanConnectBackward = true; //-X方向
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room Settings")
    bool bCanConnectRight = true; //+Y方向
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room Settings")
    bool bCanConnectLeft = true; //-Y方向
    
    UFUNCTION(BlueprintCallable)
    bool RegisterRoamingPoint(UEnemyRoamingPointComponent* registerPoint);
    UFUNCTION(BlueprintCallable)
    bool RegisterSpawnPoint(USpawnPointComponent* registerPoint);
    UFUNCTION()
    void SetDoorCanOpen(EDoorDirection Direction, bool bInCanOpen);

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    UFUNCTION()
    AAutomaticDoor* GetDoorByDirection(EDoorDirection Direction);
    UPROPERTY(EditDefaultsOnly, Category = "Room Settings") 
    ERoomType RoomType = ERoomType::Normal;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<UEnemyRoamingPointComponent*> RoamingPoints;
    UPROPERTY(BlueprintReadOnly)
    TArray<USpawnPointComponent*> SpawnPoints;

    //各方向のドアを保持するChildActorComponent
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UChildActorComponent* DoorForwardComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UChildActorComponent* DoorBackwardComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UChildActorComponent* DoorLeftComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UChildActorComponent* DoorRightComponent;
};