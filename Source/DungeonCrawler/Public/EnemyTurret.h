// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "EnemyBase.h"
#include "Components/SphereComponent.h"
#include "EnemyTurret.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRAWLER_API AEnemyTurret : public AEnemyBase
{
	GENERATED_BODY()

public:

	AEnemyTurret();
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	ABullet* Bullet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	USceneComponent* ShotTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	USphereComponent* SphereCollision; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float ShotTime = 2.0f;

	FTimerHandle ShotTimerHandle;

	virtual void HandleSeeTaget() override;
	
	UFUNCTION()
	void Shoot();
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnBullet();
	UFUNCTION(BlueprintCallable)
	void StopShotTimer();
};
