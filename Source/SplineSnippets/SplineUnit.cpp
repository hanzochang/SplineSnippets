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
	for (auto i = 0; i < Density; i++)
	{
		// つぎここ
		FVector VectorDiff = (VertexVector / WaveCycleCount / 2);

		float BranchPoint = FMath::Sin((PI / (Density / WaveCycleCount)) * 2 *  i);
		float BranchPointPrev = (i != 0 ? FMath::Sin((PI / (Density / WaveCycleCount)) * 2 * (i - 1)) : 0);
		if (BranchPoint > 0)
		{
			if (BranchPoint - BranchPointPrev > 0)
			{
				//Points.Push(BetweenPoints() * i + (VectorDiff * (i % (int)WaveCycleCount)));
				//Points.Push(BetweenPoints() * i);
				Points.Push(BetweenPoints() * i + (VectorDiff * (i% (int)(Density/WaveCycleCount/2))));
			}
			else
			{
				//Points.Push(BetweenPoints() * i + (VectorDiff * (i % (i - (int)(Density/WaveCycleCount/2)))));
			}
		}
		else if (BranchPoint < 0)
		{
			if (BranchPoint - BranchPointPrev > 0)
			{
				Points.Push(BetweenPoints() * i + ( -VectorDiff * (i% (int)(Density/WaveCycleCount/2))));
				//Points.Push(BetweenPoints() * i + (-VertexVector * (i % (int)WaveCycleCount)));
			}
			else
			{
				Points.Push(BetweenPoints() * i + (-VertexVector * (i - (i % (int)WaveCycleCount))));
			}
		}
		//else
		//{
		//	Points.Push(BetweenPoints() * i);
		//}
		
		//float VertexBase = FMath::Sin(PI / Density * i * WaveCycleCount);
		//Points.Push(BetweenPoints() * i + (VertexVector * VertexBase));
	}
}
