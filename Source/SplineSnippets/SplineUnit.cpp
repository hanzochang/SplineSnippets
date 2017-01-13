// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineSnippets.h"
#include "SplineUnit.h"

/**
 * Converts a FSplineUnit to String.
 * 
 * @param	SplineUnit		SplineUnit members to sanitize
 * @returns Strint
 */

FString FSplineUnit::ToDebugString()
{
	FString Result;
	Result =
		"Distance: " + Distance.ToString() + "  |   "  +
		"Density: " + FString::FromInt(Density) + "  |   " + 
		"VertexVector: " + VertexVector.ToString() + "  |   " + 
		"MaxWidth: " + MinWidth.ToString() + "  |   " +
		"Msec: " + FString::SanitizeFloat(Msec);

	return Result;
}

FVector FSplineUnit::BetweenPoints()
{
	return Distance / Density;
}

//SplineUnitをPointsポインタにSplineUnitの状態に応じてセットする
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
		Points.Push(BetweenPoints() * i);
	}
}

void FSplineUnit::DeriveWaveSinPoints(TArray<FVector> &Points)
{
	for (auto i = 0; i < Density; i++)
	{
		float VertexBase = FMath::Sin(PI / Density * i * WaveCycleCount);
		Points.Push(BetweenPoints() * i +(VertexVector * VertexBase));
	}
}

void FSplineUnit::DeriveWaveTrianglePoints(TArray<FVector> &Points)
{
	float Quater = Density / WaveCycleCount;

	for (auto i = 0; i < Density; i++)
	{
		FVector BasePoint = BetweenPoints() * i;
		float NumPerQuater = i / Quater;
		float NumPerQuaterDecimal = NumPerQuater - FMath::Floor(NumPerQuater);
		float BranchPoint = FMath::Sin(PI * (i / Quater));
		float BranchPointPrev = (i != 0 ? FMath::Sin(PI * ((i-1) / Quater)) : 0);

		if (BranchPoint > 0)
		{
			if (BranchPoint - BranchPointPrev >= 0)
			{
				Points.Push(BasePoint + (VertexVector * (NumPerQuaterDecimal)));
			}
			else
			{
				Points.Push(BasePoint + (VertexVector - (VertexVector * (NumPerQuaterDecimal))));
			}
		}
		else if (BranchPoint < 0)
		{
			if (BranchPoint - BranchPointPrev >= 0)
			{
				Points.Push(BasePoint - (VertexVector - (VertexVector * (NumPerQuaterDecimal))));
			}
			else
			{
				Points.Push(BasePoint - (VertexVector * (NumPerQuaterDecimal)));
			}
		}
		else
		{
			Points.Push(BetweenPoints() * i);
		}
		
	}
}
