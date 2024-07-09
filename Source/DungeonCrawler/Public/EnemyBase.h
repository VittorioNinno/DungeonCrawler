// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "EnemyBase.generated.h"



UENUM(BlueprintType)
enum EEnemyState :uint8
{
	Patrolling UMETA(DisplayName = "Patrolling"),
	Fight UMETA(DisplayName = "Fight")
};


UCLASS()
class DUNGEONCRAWLER_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBehaviorTreeComponent* EnemyBehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;
	
	// Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	AActor* CurrentTarget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fight")
	TEnumAsByte<EEnemyState> EnemyState;
	
	int32 CurrentHealth;
	bool IsDead;

	
	UFUNCTION(BlueprintCallable)
	void SetEnemyComponents(UBehaviorTreeComponent* InBehaviorTree, UPawnSensingComponent* InSensingComponent);

	UFUNCTION()
	void SetTarget(APawn* InTarget);

	virtual void HandleSeeTaget();

public:
	
	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(int32 DamageToReceive);
	UFUNCTION(BlueprintCallable)
	void HandleDeath();
};