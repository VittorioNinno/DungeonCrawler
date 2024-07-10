// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTurret.h"

#include "Bullet.h"
#include "Chaos/Deformable/ChaosDeformableCollisionsProxy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AEnemyTurret::AEnemyTurret()
{
	ShotTransform = CreateDefaultSubobject<USceneComponent>("ShotTransform");
	ShotTransform->SetupAttachment(RootComponent);

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetupAttachment(RootComponent);
}

void AEnemyTurret::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(CurrentTarget)
	{
		FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), CurrentTarget->GetActorLocation());
		SetActorRotation(LookRot);
	}
}

void AEnemyTurret::HandleSeeTaget()
{
	Super::HandleSeeTaget();
	GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, this, &AEnemyTurret::Shoot, ShotTime, true);
}

void AEnemyTurret::Shoot()
{
	SpawnBullet();
}

void AEnemyTurret::StopShotTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(ShotTimerHandle);
}


