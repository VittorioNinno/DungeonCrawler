#include "Drone.h"

#include "Components/SplineComponent.h"

ADrone::ADrone(): Player(nullptr), DynamicMaterial(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Create Floating Movement Component
	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));
	
	// Initialize Movement Variables
	SplinePosition = 0.0f;
	MovementSpeed = 100.0f;
	MinHeight = 320.0f;

	// Initialize State Tree Variables
	CurrentState = EDroneState::Patrol;
	TimeSinceLastSeenPlayer = 0.0f;
	ReturnDelay = 3.0f;
	bIsPlayerVisible = false;

	// Initialize Material Variables
	ChaseEmissiveColor = FLinearColor::Red;
}

void ADrone::BeginPlay()
{
	Super::BeginPlay();

	// Find Vision Cone
	VisionCone = FindComponentByClass<UStaticMeshComponent>();
	if (VisionCone)
	{
		// Bind Overlap Events
		VisionCone->OnComponentBeginOverlap.AddDynamic(this, &ADrone::OnVisionConeOverlap);
		VisionCone->OnComponentEndOverlap.AddDynamic(this, &ADrone::OnVisionConeEndOverlap);

		if (UMaterialInterface* Material = VisionCone->GetMaterial(0))
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			VisionCone->SetMaterial(0, DynamicMaterial);

			// Store Original Emissive Color
			OriginalEmissiveColor = DynamicMaterial->K2_GetVectorParameterValue("Emissive Color");
		}
	}
}

void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (CurrentState)
	{
	case EDroneState::Patrol:
		DynamicMaterial->SetVectorParameterValue("Emissive Color", OriginalEmissiveColor);
		MoveAlongSpline(DeltaTime);
		break;
		
	case EDroneState::Chase:
		DynamicMaterial->SetVectorParameterValue("Emissive Color", ChaseEmissiveColor);
		ChasePlayer(DeltaTime);
		break;

	case EDroneState::Return:
		DynamicMaterial->SetVectorParameterValue("Emissive Color", OriginalEmissiveColor);
		ReturnToSpline(DeltaTime);
		break;

	default:
		break;
	}

	// Update Last Known Player Location if Player is Visible
	if (bIsPlayerVisible && CurrentState == EDroneState::Chase)
	{
		if(Player)
		{
			LastKnownPlayerLocation = Player->GetActorLocation();
		}

		TimeSinceLastSeenPlayer = 0.0f;
	}
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADrone::MoveAlongSpline(const float DeltaTime)
{
	if (SplineActor)
	{
		if (const USplineComponent* SplineComponent = SplineActor->FindComponentByClass<USplineComponent>())
		{
			// Update Position Along Spline
			SplinePosition += DeltaTime * MovementSpeed;
			const float NextPosition = SplinePosition + 10.0f;
			FVector NewLocation = SplineComponent->GetLocationAtDistanceAlongSpline(SplinePosition, ESplineCoordinateSpace::World);
			const FVector NextLocation = SplineComponent->GetLocationAtDistanceAlongSpline(NextPosition, ESplineCoordinateSpace::World);

			// Calculate Direction and Target Rotation
			const FVector Direction = (NextLocation - NewLocation).GetSafeNormal();
			const FRotator TargetRotation = Direction.Rotation();
			
			// Ensure Drone Minimum Height
			if (NewLocation.Z < MinHeight)
			{
				NewLocation.Z = MinHeight;
			}

			// Interpolate the current rotation to the target rotation
			const FRotator CurrentRotation = GetActorRotation();
			const FRotator SmoothedRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 3.0f);

			// Move and Rotate Drone
			SetActorLocationAndRotation(NewLocation, SmoothedRotation);

			// Loop Spline
			if (const float SplineLength = SplineComponent->GetSplineLength(); SplinePosition > SplineLength)
			{
				SplinePosition = 0.0f;
			}
		}
	}
}

void ADrone::ChasePlayer(float DeltaTime)
{
	// Move Towards Last Known Player Location
	FVector Direction = (LastKnownPlayerLocation - GetActorLocation()).GetSafeNormal();
	Direction.Z = 0; // Ignore Z Axis
	FVector NewLocation = GetActorLocation() + Direction * MovementSpeed * DeltaTime;

	// Maintain Minimum Height
	if (NewLocation.Z < MinHeight)
	{
		NewLocation.Z = MinHeight;
	}

	// Calculate and Set New Rotation
	const FRotator TargetRotation = Direction.Rotation();
	const FRotator CurrentRotation = GetActorRotation();
	const FRotator SmoothedRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 2.0f);
	
	// Move and Rotate Drone
	SetActorLocationAndRotation(NewLocation, SmoothedRotation);

	if (TimeSinceLastSeenPlayer >= ReturnDelay)
	{
		ChangeState(EDroneState::Return);
	}
	else
	{
		TimeSinceLastSeenPlayer += DeltaTime;
	}
}

void ADrone::ReturnToSpline(float DeltaTime)
{
    if (SplineActor)
    {
        if (const USplineComponent* SplineComponent = SplineActor->FindComponentByClass<USplineComponent>())
        {
            // Get Start Location of Spline
            FVector StartLocation = SplineComponent->GetLocationAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);

            // Ensure Drone's Z Position is Maintained
            StartLocation.Z = GetActorLocation().Z;

            // Move Towards Start of Spline
            FVector Direction = (StartLocation - GetActorLocation()).GetSafeNormal();
            Direction.Z = 0; // Ignore Z Axis
            FVector NewLocation = GetActorLocation() + Direction * MovementSpeed * DeltaTime;

            // Maintain Minimum Height
            if (NewLocation.Z < MinHeight)
            {
                NewLocation.Z = MinHeight;
            }

            // Calculate and Set New Rotation
            const FRotator TargetRotation = Direction.Rotation();
            const FRotator CurrentRotation = GetActorRotation();
            const FRotator SmoothedRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 0.7f);

            // Move and Rotate Drone
            SetActorLocationAndRotation(NewLocation, SmoothedRotation);

            // If Close Enough, Switch to Patrol State
            FVector FlatNewLocation = NewLocation;
            FVector FlatStartLocation = StartLocation;

        	// Ignore Z in Distance Calculation
            FlatNewLocation.Z = 0;
            FlatStartLocation.Z = 0;
            
            if (FVector::Dist(FlatNewLocation, FlatStartLocation) < 10.0f)
            {
                SplinePosition = 0.0f;
                ChangeState(EDroneState::Patrol);
            }
        }
    }
}


void ADrone::OnVisionConeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check If Overlapping Actor Has "Player" Tag
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		// Update Last Known Player Location
		Player = OtherActor;
		LastKnownPlayerLocation = OtherActor->GetActorLocation();
		bIsPlayerVisible = true;

		// Switch to Chase State
		ChangeState(EDroneState::Chase);
		TimeSinceLastSeenPlayer = 0.0f;
	}
}

void ADrone::OnVisionConeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if Actor Leaving Vision Cone Has "Player" Tag
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		bIsPlayerVisible = false;
	}
}