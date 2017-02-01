// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SplineUnit.h"
#include "Components/SplineComponent.h"
#include "SplineActorObserver.generated.h"

/**
 * 
 */
UCLASS()
class SPLINESNIPPETS_API USplineActorObserver : public UObject
{
	GENERATED_BODY()
	
public:

	float TotalSplineUnitLength = 0;
	float CurrentSplineUnitLength = 0;
	int32 CurrentToSplineUnitNum = 0;

	int32 PrevSplineUnitPointStartNum = 0;
	int32 PrevSplineUnitPointEndNum = 0;

	// “¯‚É•\¦‰Â”\‚ÈSplineUnit‚ÌÅ‘å”‚ğİ’è‚·‚é
	TArray<float> DisplayableSplineUnitLengths;
	int32 DisplayableSplineUnitLimit = 2;
	float DisplayableSplineUnitLength = 0;

public:

	USplineActorObserver();

	void AssignSplinePointsFromSplineUnit(USplineComponent *Spline,
		TArray<FVector> &SplinePoints,
		TArray<FSplineUnit> &SplineUnits);

private:

	void AssignSplinePoints(USplineComponent *Spline,
		TArray<FVector> &SplinePoints,
		TArray<FSplineUnit> &SplineUnits,
		int32 counter,
		FVector &StartPoint
	);

	float GetCurrentSplineUnitLength(
		USplineComponent *Spline,
		int32 PointStartNumber,
		int32 PointEndNumber
	);

	float GetGeneratedSplineLength(
		USplineComponent *Spline,
		int32 EdgeSplineUnitStartSplineNum
	);

	void SetDisplayableSplineUnitLength();
};
