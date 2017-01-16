// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SplineUnit.h"
#include "Components/SplineComponent.h"
#include "SplineActor.generated.h"


UCLASS()
class SPLINESNIPPETS_API ASplineActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnyWhere)
	USplineComponent* MySpline;

	UPROPERTY(VisibleAnyWhere)
	UStaticMeshComponent* SM;

private:

	void SetDebugGridsEachSplinePoints(int PointNum);
	void ASplineActor::LoadDebugGrid();
	//TArray<FSplineUnit> GenerateInitialSplineUnits(int TestNum);
	TArray<FSplineUnit> GenerateInitialSplineUnits(int TestNum, ESplineUnit WaveType);
	
public:	
	// Sets default values for this actor's properties
	ASplineActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnyWhere)
	TArray<FSplineUnit> SplineUnits;
	
	UPROPERTY(EditAnywhere, Category = General)
    TSubclassOf<class AActor> WhatToSpawn;
};
