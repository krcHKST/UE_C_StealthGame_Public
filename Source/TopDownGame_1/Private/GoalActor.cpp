// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalActor.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownGame_1/TopDownGame_1Character.h"

// Sets default values
AGoalActor::AGoalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GoalCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	GoalCollider->OnComponentBeginOverlap.AddDynamic(this,&AGoalActor::BeginOverlap);
}

void AGoalActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATopDownGame_1Character* PlayerCharacter = Cast<ATopDownGame_1Character>(OtherActor);
	if (PlayerCharacter)
	{
		UGameplayStatics::OpenLevel(this,OpenLevelName);
	}
}

// Called when the game starts or when spawned
void AGoalActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
