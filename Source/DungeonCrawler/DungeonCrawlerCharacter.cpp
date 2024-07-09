// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonCrawlerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

ADungeonCrawlerCharacter::ADungeonCrawlerCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Add "Player" tag
	Tags.Add(FName("Player"));

	// Initialize Movement Variable
	MovementSpeed = 250.0f;
	SprintSpeed = 550.0f;

	// Initialize Camera Variable
	CameraMoveSpeed = 500.0f;
	MaxCameraOffsetX = 500.0f;
	MaxCameraOffsetY = 500.0f;
}

void ADungeonCrawlerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Save initial camera boom settings
	InitialCameraBoomLocation = CameraBoom->GetComponentLocation();
	InitialCameraBoomRotation = CameraBoom->GetComponentRotation();
	InitialCameraBoomArmLength = CameraBoom->TargetArmLength;

	// Set Default Movement Speed
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void ADungeonCrawlerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ADungeonCrawlerCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ADungeonCrawlerCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ADungeonCrawlerCharacter::StartSprint() const
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ADungeonCrawlerCharacter::StopSprint() const
{
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void ADungeonCrawlerCharacter::LookUp(float Value) const
{
	if (Value != 0.0f)
	{
		FVector NewCameraPosition = TopDownCameraComponent->GetRelativeLocation();
		float NewZ = FMath::Clamp(NewCameraPosition.Z + Value * CameraMoveSpeed * GetWorld()->GetDeltaSeconds(), -MaxCameraOffsetX, MaxCameraOffsetX);
		NewCameraPosition.Z = NewZ;
		TopDownCameraComponent->SetRelativeLocation(NewCameraPosition);
	}
}

void ADungeonCrawlerCharacter::LookRight(float Value) const
{
	if (Value != 0.0f)
	{
		FVector NewCameraPosition = TopDownCameraComponent->GetRelativeLocation();
		float NewY = FMath::Clamp(NewCameraPosition.Y + Value * CameraMoveSpeed * GetWorld()->GetDeltaSeconds(), -MaxCameraOffsetY, MaxCameraOffsetY);
		NewCameraPosition.Y = NewY;
		TopDownCameraComponent->SetRelativeLocation(NewCameraPosition);
	}
}

void ADungeonCrawlerCharacter::ResetCamera() const
{
	if (CameraBoom != nullptr)
	{
		// Calculate the new boom location based on the current actor location
		FVector NewBoomLocation = GetActorLocation();

		// Reset the camera boom to the new location
		CameraBoom->SetWorldLocation(NewBoomLocation);
		CameraBoom->SetRelativeRotation(InitialCameraBoomRotation);
		CameraBoom->TargetArmLength = InitialCameraBoomArmLength;
	}
	
	if (TopDownCameraComponent != nullptr)
	{
		// Ensure the camera component itself is correctly centered relative to the boom
		TopDownCameraComponent->SetRelativeLocation(FVector(0, 0, 0));
	}
}





