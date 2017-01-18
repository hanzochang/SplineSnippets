// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineSnippets.h"
#include "SplineMoveComponent.h"


// Sets default values for this component's properties
USplineMoveComponent::USplineMoveComponent()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	DeltaTimeSum = 0;
}


// Called when the game starts
void USplineMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void USplineMoveComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	DeltaTimeSum += DeltaTime;
	ForceMove();
}

void USplineMoveComponent::SetSpline(USplineComponent *Spline)
{
	MySpline = Spline;
}

void USplineMoveComponent::ForceMove()
{
	AActor* Owner = GetOwner();
	FVector Location = GetCurrentLocationAlongSpline(DeltaTimeSum, MySpline);
	FVector Direction = GetCurrentDirectionAlongSpline(DeltaTimeSum, MySpline);
	FRotator Rotation = GetCurrentRotationAlongSpline(DeltaTimeSum, MySpline);
	FQuat Quaternion = FQuat{ Direction.X, Direction.Y, Direction.Z, 0 };
	Owner->SetActorLocationAndRotation(Location, Quaternion);
}

FVector USplineMoveComponent::GetCurrentLocationAlongSpline(float distance, USplineComponent *Spline)
{
	return Spline->GetWorldLocationAtDistanceAlongSpline(distance);
}

FVector USplineMoveComponent::GetCurrentDirectionAlongSpline(float distance, USplineComponent *Spline)
{
	return Spline->GetWorldDirectionAtDistanceAlongSpline(distance);
}

FRotator USplineMoveComponent::GetCurrentRotationAlongSpline(float distance, USplineComponent *Spline)
{
	return Spline->GetWorldRotationAtDistanceAlongSpline(distance);
}