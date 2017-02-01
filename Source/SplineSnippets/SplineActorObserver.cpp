// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineSnippets.h"
#include "SplineActorObserver.h"

USplineActorObserver::USplineActorObserver()
{
}


void USplineActorObserver::AssignSplinePointsFromSplineUnit(USplineComponent *Spline,
	TArray<FVector> &SplinePoints,
	TArray<FSplineUnit> &SplineUnits)
{
	FVector StartPoint = FVector{ 0,0,0 };
	int32 EdgeSplineUnitStartSplineNum = 0;
	int32 counter = 0;

	for (auto i = 0; i < DisplayableSplineUnitLimit; i++)
	{
		EdgeSplineUnitStartSplineNum = Spline->GetNumberOfSplinePoints();
		counter += i;
		if (counter >= SplineUnits.Num()) { counter = 0; }

		AssignSplinePoints(Spline, SplinePoints, SplineUnits, counter, StartPoint);
		DisplayableSplineUnitLengths.Push(GetGeneratedSplineLength(Spline, EdgeSplineUnitStartSplineNum));
	}

	SetDisplayableSplineUnitLength();
}

void USplineActorObserver::AssignSplinePoints(
	USplineComponent *Spline,
	TArray<FVector> &SplinePoints,
	TArray<FSplineUnit> &SplineUnits,
	int32 counter,
	FVector &StartPoint
)
{
	SplineUnits[counter].DeriveSplinePointsAddTo(SplinePoints, StartPoint);
	Spline->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::Type::Local);
	StartPoint = Spline->GetLocationAtSplinePoint(Spline->GetNumberOfSplinePoints(),
		ESplineCoordinateSpace::Local);
}


float USplineActorObserver::GetCurrentSplineUnitLength(
	USplineComponent *Spline,
	int32 PointStartNumber,
	int32 PointEndNumber
)
{
	float LastLength = Spline->GetDistanceAlongSplineAtSplinePoint(PointEndNumber);
    float StartLength = Spline->GetDistanceAlongSplineAtSplinePoint(PointStartNumber);
	return LastLength - StartLength;
}

float USplineActorObserver::GetGeneratedSplineLength(
	USplineComponent *Spline,
	int32 EdgeSplineUnitStartSplineNum
)
{
	PrevSplineUnitPointStartNum = EdgeSplineUnitStartSplineNum;
	PrevSplineUnitPointEndNum = Spline->GetNumberOfSplinePoints() - 1;
	return GetCurrentSplineUnitLength(Spline, PrevSplineUnitPointStartNum, PrevSplineUnitPointEndNum);
}

void USplineActorObserver::SetDisplayableSplineUnitLength()
{
	for(auto i = 0; i <= CurrentToSplineUnitNum; i++)
	{
		DisplayableSplineUnitLength += DisplayableSplineUnitLengths[i];
	}
}