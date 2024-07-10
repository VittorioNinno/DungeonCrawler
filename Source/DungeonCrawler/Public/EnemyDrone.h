// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyDroneSplineToFollow.h"
#include "EnemyBase.h"
#include "IHackable.h"
#include "Components/SplineComponent.h"
#include "Components/SpotLightComponent.h"
#include "EnemyDrone.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRAWLER_API AEnemyDrone : public AEnemyBase, public IIHackable
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual  void HandleSeeTaget() override;

	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USpotLightComponent* SpotLightComponent;
	// Movements
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	AEnemyDroneSplineToFollow* ActorSplineToFollow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	USplineComponent* SplineToFollow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementTimer;
	
	float CurrentMovementTime;

	UFUNCTION(BlueprintImplementableEvent)
	void FollowTarget(FVector LocationToFollow);

	virtual void HandleHacking() override;
};