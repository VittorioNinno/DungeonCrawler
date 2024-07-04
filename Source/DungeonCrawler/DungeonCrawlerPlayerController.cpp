// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonCrawlerPlayerController.h"

#include "DungeonCrawlerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ADungeonCrawlerPlayerController::ADungeonCrawlerPlayerController(): ShortPressThreshold(0),
                                                                    DefaultMappingContext(nullptr),
                                                                    MoveForwardAction(nullptr),
                                                                    MoveRightAction(nullptr), SprintAction(nullptr),
                                                                    bMoveToMouseCursor(0), bIsTouch(false)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void ADungeonCrawlerPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ADungeonCrawlerPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ADungeonCrawlerPlayerController::MoveForward);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ADungeonCrawlerPlayerController::MoveRight);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ADungeonCrawlerPlayerController::ToggleSprint);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADungeonCrawlerPlayerController::MoveForward(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ADungeonCrawlerCharacter* ControlledCharacter = Cast<ADungeonCrawlerCharacter>(ControlledPawn))
		{
			ControlledCharacter->MoveForward(Value.Get<float>());
		}
	}
}

void ADungeonCrawlerPlayerController::MoveRight(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ADungeonCrawlerCharacter* ControlledCharacter = Cast<ADungeonCrawlerCharacter>(ControlledPawn))
		{
			ControlledCharacter->MoveRight(Value.Get<float>());
		}
	}
}

void ADungeonCrawlerPlayerController::ToggleSprint()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ADungeonCrawlerCharacter* ControlledCharacter = Cast<ADungeonCrawlerCharacter>(ControlledPawn))
		{
			if (ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed == ControlledCharacter->GetMovementSpeed())
			{
				ControlledCharacter->StartSprint();
			}
			else
			{
				ControlledCharacter->StopSprint();
			}
		}
	}
}
