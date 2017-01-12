// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineSnippets.h"
#include "SplineActor.h"

// Sets default values
ASplineActor::ASplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Static Mesh include Spline Component
	SetRootComponent(CreateDefaultSubobject<UStaticMeshComponent>(FName("SM")));
    MySpline = CreateDefaultSubobject<USplineComponent>(FName("MySpline"));
	MySpline->SetupAttachment(SM);

	LoadDebugGrid();
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();
	SplineUnits = GenerateInitialSplineUnits(1);

	TArray<FVector> SplinePoints;
	for (auto SplineUnit : SplineUnits)
	{
		SplineUnit.DeriveSplinePointsAddTo(SplinePoints);
	};

	MySpline->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::Type::Local);

	for (auto i = 0 ; i < MySpline->GetNumSplinePoints(); i++) {
		SetDebugGridsEachSplinePoints(i);
	}

	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor(255, 255, 255, 255), FString::FromInt(MySpline->GetNumSplinePoints()));
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor(255, 0, 0, 255), FString::FromInt(SplineUnits.Num()));
}

// Called every frame
void ASplineActor::Tick( float DeltaTime )
{
	//UE_LOG(LogTemp, Warning, TEXT("output : %s"), L"ÉçÉOÇÃì‡óe");
	Super::Tick( DeltaTime );

}

TArray<FSplineUnit> ASplineActor::GenerateInitialSplineUnits(int TestNum)
{
	TArray<FSplineUnit> SplineUnitsTmp;

	//for (auto i = 0; i < TestNum; i++) {
	//	FSplineUnit SplineUnitTmp;
		
	//	SplineUnitTmp.WaveType = ESplineUnit::WAVE_LINEAR;
	//	SplineUnitTmp.Msec = 10.0f;
	//	SplineUnitTmp.MaxWidth = 0;
	//	SplineUnitTmp.MinWidth = 0;
	//	SplineUnitTmp.Density = 10;

	//	FVector UnitVector = FVector(0, 500.0f, 0);
	//	if (i % 2 == 1) {
	//		SplineUnitTmp.Distance = UnitVector * i * -1;
	//	}
	//	else {
	//		SplineUnitTmp.Distance = UnitVector * i;
	//	}

	//	SplineUnitsTmp.Push(SplineUnitTmp);
	//	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor(0, 0, 255, 255), FString::FromInt(SplineUnitsTmp.Num()));
    //}

	for (auto i = 0; i < TestNum; i++) {
		FSplineUnit SplineUnitTmp;
		
		SplineUnitTmp.WaveType = ESplineUnit::WAVE_TRIANGLE;
		SplineUnitTmp.Msec = 10.0f;
		SplineUnitTmp.VertexVector = FVector(0, 100.0f, 0);
		SplineUnitTmp.MinWidth = FVector(1000.0f, 0, 0);
		SplineUnitTmp.WaveCycleCount = 4.0f;
		SplineUnitTmp.Density = 500;
		SplineUnitTmp.Distance = FVector(5000.0f, 0, 0);

		SplineUnitsTmp.Push(SplineUnitTmp);
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor(0, 0, 255, 255), FString::FromInt(SplineUnitsTmp.Num()));
    }

	return SplineUnitsTmp;
}


void ASplineActor::LoadDebugGrid()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> DebugGridClassFinder( TEXT( "Blueprint'/Game/T_DebugGrid.T_DebugGrid'" ) );

	if (DebugGridClassFinder.Succeeded())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor(255, 0, 0, 255), TEXT("Hello World: "));
		WhatToSpawn = (UClass*)DebugGridClassFinder.Object->GeneratedClass;
	}
}

void ASplineActor::SetDebugGridsEachSplinePoints(int PointNum)
{
	FVector Location = { 0.0, 0.0, 0.0 };
	FRotator Rotation = { 0.0, 0.0, 0.0 };
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	AActor* const SpawningObject = GetWorld()->SpawnActor<AActor>(WhatToSpawn,
		MySpline->GetLocationAtSplinePoint(PointNum, ESplineCoordinateSpace::Type::Local), Rotation, SpawnParams);
}