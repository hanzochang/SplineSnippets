// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineSnippets.h"
#include "SplineUnit.h"

/**
 * Converts a FSplineUnit to String.
 * 
 * @param	SplineUnit		SplineUnit members to sanitize
 * @returns Strint
 */

FString FSplineUnit::ToString(FSplineUnit SplineUnit)
{
	FString Result;
	Result =
		"Distance: " + SplineUnit.Distance.ToString() + "  |   "  +
		"Density: " + FString::FromInt(SplineUnit.Density) + "  |   " + 
		"MaxWidth: " + FString::SanitizeFloat(SplineUnit.MaxWidth) + "  |   " + 
		"MaxWidth: " + FString::SanitizeFloat(SplineUnit.MinWidth) + "  |   " +
		"Msec: " + FString::SanitizeFloat(SplineUnit.Msec);

	return Result;
}

FVector FSplineUnit::BetweenPoints(FSplineUnit SplineUnit)
{
	return SplineUnit.Distance / SplineUnit.Density;
}
