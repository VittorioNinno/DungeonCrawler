// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDrone.h"

void AEnemyDrone::BeginPlay()
{
	Super::BeginPlay();

	if(ActorSplineToFollow)
	{
		SplineToFollow = ActorSplineToFollow->SplineComponent;
	}
}

void AEnemyDrone::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (SplineToFollow && !CurrentTarget)
	{
		if (CurrentMovementTime < MovementTimer)
		{
			CurrentMovementTime += DeltaSeconds;
			float Distace = SplineToFollow->GetSplineLength() * (CurrentMovementTime / MovementTimer);
			FVector Location = SplineToFollow->GetLocationAtDistanceAlongSpline(Distace, ESplineCoordinateSpace::World);
			FRotator Rotation = SplineToFollow->GetRotationAtDistanceAlongSpline(Distace, ESplineCoordinateSpace::World);

			SetActorLocation(Location);
			SetActorRotation(Rotation);

			if(CurrentMovementTime >= MovementTimer)
			{
				CurrentMovementTime = 0;
			}
		}
	}
}

void AEnemyDrone::HandleSeeTaget()
{
	Super::HandleSeeTaget();
}