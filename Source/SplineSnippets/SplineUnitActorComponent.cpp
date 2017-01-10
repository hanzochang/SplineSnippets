// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineSnippets.h"
#include "SplineUnitActorComponent.h"

USplineUnitActorComponent::USplineUnitActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void USplineUnitActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USplineUnitActorComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

FString USplineUnitActorComponent::ToDebugString()
{
	FString Result;
	Result =
		"Distance: " + Distance.ToString() + "  |   "  +
		"Density: " + FString::FromInt(Density) + "  |   " + 
		"MaxWidth: " + FString::SanitizeFloat(MaxWidth) + "  |   " + 
		"MaxWidth: " + FString::SanitizeFloat(MinWidth) + "  |   " +
		"Msec: " + FString::SanitizeFloat(Msec);

	return Result;
}

FVector USplineUnitActorComponent::BetweenPoints()
{
	return Distance / Density;
}
