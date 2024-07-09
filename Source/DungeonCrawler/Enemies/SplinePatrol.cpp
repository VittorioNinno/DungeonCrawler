#include "SplinePatrol.h"

#include "Components/SplineComponent.h"

ASplinePatrol::ASplinePatrol()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Spline Component
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComponent;

	// Initialize Spline Points
	SplineRadius = 500.0f;
	InitializeSpline();
}

void ASplinePatrol::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASplinePatrol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASplinePatrol::InitializeSpline() const
{
	if (SplineComponent)
	{
		// Clear Existing Points
		SplineComponent->ClearSplinePoints();

		// Add Points to Form a Circle
		for (int32 i = 0; i < 4; ++i)
		{
			const float Angle = FMath::DegreesToRadians(90.0f * i);
			FVector PointLocation = FVector(SplineRadius * FMath::Cos(Angle), SplineRadius * FMath::Sin(Angle), 0.0f);
			SplineComponent->AddSplinePoint(PointLocation, ESplineCoordinateSpace::Local);
		}

		// Set Spline to be Closed Loop
		SplineComponent->SetClosedLoop(true);
	}
}

