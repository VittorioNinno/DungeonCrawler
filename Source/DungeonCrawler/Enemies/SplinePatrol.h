#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplinePatrol.generated.h"

class USplineComponent;

UCLASS()
class DUNGEONCRAWLER_API ASplinePatrol : public AActor
{
	GENERATED_BODY()

public:
	ASplinePatrol();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Spline Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineComponent* SplineComponent;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Components", meta = (AllowPrivateAccess = true))
	float SplineRadius;
	
	void InitializeSpline() const;
};
