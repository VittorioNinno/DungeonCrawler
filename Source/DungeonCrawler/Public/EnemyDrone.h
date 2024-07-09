// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyDroneSplineToFollow.h"
#include "EnemyBase.h"
#include "Components/SplineComponent.h"
#include "EnemyDrone.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRAWLER_API AEnemyDrone : public AEnemyBase
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual  void HandleSeeTaget() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	AEnemyDroneSplineToFollow* ActorSplineToFollow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	USplineComponent* SplineToFollow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeedOnSpline;

	float LocationInSpline = 0;

	float CurrentMovementTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementTimer;


};