// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDroneSplineToFollow.h"

// Sets default values
AEnemyDroneSplineToFollow::AEnemyDroneSplineToFollow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SplineComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyDroneSplineToFollow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyDroneSplineToFollow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}