// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRoamingPoint.h"

// Sets default values
AEnemyRoamingPoint::AEnemyRoamingPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemyRoamingPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyRoamingPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

