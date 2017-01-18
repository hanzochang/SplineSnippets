// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "SplineMoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPLINESNIPPETS_API USplineMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnyWhere)
	USplineComponent* MySpline;

	float DeltaTimeSum;

public:	
	// Sets default values for this component's properties
	USplineMoveComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void SetSpline(USplineComponent *Spline);

	void ForceMove();
	
	FVector GetCurrentLocationAlongSpline(float distance, USplineComponent *Spline);

	FVector GetCurrentDirectionAlongSpline(float distance, USplineComponent *Spline);

	FRotator GetCurrentRotationAlongSpline(float distance, USplineComponent *Spline);
};
