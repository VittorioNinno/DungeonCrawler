#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "Drone.generated.h"

UENUM(BlueprintType)
enum class EDroneState : uint8
{
	Patrol     UMETA(DisplayName = "Patrol"),
	Chase      UMETA(DisplayName = "Chase"),
	Return     UMETA(DisplayName = "Return")
};

UCLASS()
class DUNGEONCRAWLER_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	ADrone();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Movement Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UFloatingPawnMovement* FloatingMovement;
	
private:
	// Minimum Height Drone Can Fly
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	float MinHeight;

	// Movement Speed of Drone
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	float MovementSpeed;

	// Reference to Spline Patrol Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	AActor* SplineActor;

	float SplinePosition;

	// Reference to Vision Cone Component
	UPROPERTY()
	UStaticMeshComponent* VisionCone;

	// Overlap Event Handlers for Vision Cone
	UFUNCTION()
	void OnVisionConeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnVisionConeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Current state of the drone
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State Tree", meta = (AllowPrivateAccess = true))
	EDroneState CurrentState;

	// Reference to the player
	UPROPERTY()
	AActor* Player;

	FVector LastKnownPlayerLocation;
	float TimeSinceLastSeenPlayer;
	float ReturnDelay;
	bool bIsPlayerVisible;

	// Behaviour Tree Methods
	void MoveAlongSpline(float DeltaTime);
	void ChasePlayer(float DeltaTime);
	void ReturnToSpline(float DeltaTime);
	void ChangeState(EDroneState NewState) { CurrentState = NewState; }

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
	FLinearColor OriginalEmissiveColor;
	FLinearColor ChaseEmissiveColor;
};
