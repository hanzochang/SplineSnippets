// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineSnippets.h"
#include "SplineUnit.h"

/**
 * Converts a ESplineUnit to String.
 * 
 * @param	EplineUnit
 * @returns String
 */

FString GetSplineUnitEnumAsString(ESplineUnit EnumValue)
{
  const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESplineUnit"), true);
  if(!EnumPtr) return FString("Invalid");
 
  return EnumPtr->GetEnumName((int32)EnumValue);
};

/**
 * Converts a FSplineUnit to String.
 * 
 * @param	SplineUnit		SplineUnit members to sanitize
 * @returns String
 */

FString FSplineUnit::ToDebugString()
{
	FString Result;
	Result =
		"Distance: " + Distance.ToString() + "  |   "  +
		"Density: " + FString::FromInt(Density) + "  |   " + 
		"VertexVector: " + VertexVector.ToString() + "  |   " + 
		"Msec: " + FString::SanitizeFloat(Msec);

	return Result;
}

FVector FSplineUnit::BetweenPoints()
{
	return Distance / Density;
}

// Unreal C++内で生成するための疑似コンストラクタ
FSplineUnit FSplineUnit::GenerateSplineUnit(
		ESplineUnit WaveType,
		FVector Distance,
		FVector StartLocation,
		FVector VertexVector,
		float WaveCycleCount,
		int32 Density,
	    float Msec
)
{
	FSplineUnit SplineUnit;
	SplineUnit.WaveType = WaveType;
	SplineUnit.Msec = Msec;
	SplineUnit.VertexVector = VertexVector;
	SplineUnit.WaveCycleCount = WaveCycleCount;
	SplineUnit.Density = Density;
	SplineUnit.Distance = Distance;
	SplineUnit.StartLocation = StartLocation;

	return SplineUnit;
}

// SplineUnitをPointsポインタにSplineUnitの状態に応じてセットする
void FSplineUnit::DeriveSplinePointsAddTo(TArray<FVector> &Points)
{
	switch (WaveType)
	{
	case ESplineUnit::WAVE_LINEAR:
		DeriveWaveLinearPoints(Points);
		break;
	case ESplineUnit::WAVE_SIN:
		DeriveWaveSinPoints(Points);
		break;
	case ESplineUnit::WAVE_TRIANGLE:
		DeriveWaveTrianglePoints(Points);
		break;
	case ESplineUnit::WAVE_SAWTOOTH:
		break;
	}
}

void FSplineUnit::DeriveWaveLinearPoints(TArray<FVector> &Points)
{
	for (auto i = 0; i < Density; i++)
	{
		Points.Push(StartLocation + BetweenPoints() * i);
	}
}

void FSplineUnit::DeriveWaveSinPoints(TArray<FVector> &Points)
{
	for (auto i = 0; i < Density; i++)
	{
		float VertexBase = FMath::Sin(PI / Density * i * WaveCycleCount);
		Points.Push(StartLocation + BetweenPoints() * i +(VertexVector * VertexBase));
	}
}

void FSplineUnit::DeriveWaveTrianglePoints(TArray<FVector> &Points)
{
	float Quater = Density / WaveCycleCount;

	for (auto i = 0; i < Density; i++)
	{
		FVector BasePoint = StartLocation + BetweenPoints() * i;
		float NumPerQuater = i / Quater;
		float NumPerQuaterDecimal = NumPerQuater - FMath::Floor(NumPerQuater);
		float BranchPoint = FMath::Sin(PI * (i / Quater));
		float BranchPointPrev = (i != 0 ? FMath::Sin(PI * ((i-1) / Quater)) : 0);

		if (BranchPoint > 0)
		{
			if (BranchPoint - BranchPointPrev >= 0)
			{
				Points.Push(BasePoint + (VertexVector*2 * (NumPerQuaterDecimal)));
			}
			else
			{
				Points.Push(BasePoint + (VertexVector*2 - (VertexVector*2 * (NumPerQuaterDecimal))));
			}
		}
		else if (BranchPoint < 0)
		{
			if (BranchPoint - BranchPointPrev >= 0)
			{
				Points.Push(BasePoint - (VertexVector*2 - (VertexVector*2 * (NumPerQuaterDecimal))));
			}
			else
			{
				Points.Push(BasePoint - (VertexVector*2 * (NumPerQuaterDecimal)));
			}
		}
		else
		{
			Points.Push(BetweenPoints() * i);
		}
		
	}
}
