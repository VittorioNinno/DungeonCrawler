// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyBase::SetTarget);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::SetEnemyComponents(UBehaviorTreeComponent* InBehaviorTree, UPawnSensingComponent* InSensingComponent)
{
	EnemyBehaviorTree = InBehaviorTree;
	PawnSensingComponent = InSensingComponent;
}

void AEnemyBase::SetTarget(APawn* InTarget)
{
	if(InTarget == UGameplayStatics::GetPlayerPawn(this, 0))
	{
		CurrentTarget = InTarget;
		EnemyState = EEnemyState::Fight;
		HandleSeeTaget();
	}
}

void AEnemyBase::HandleSeeTaget()
{
}

void AEnemyBase::ReceiveDamage(int32 DamageToReceive)
{
	CurrentHealth -= DamageToReceive;
	if(CurrentHealth <= 0) HandleDeath();
}

void AEnemyBase::HandleDeath()
{
	GetCharacterMovement()->DisableMovement();
	IsDead = true;
}