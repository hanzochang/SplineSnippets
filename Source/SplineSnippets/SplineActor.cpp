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

	FString FullPath = FPaths::GameContentDir();
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor(255, 255, 255, 255), FullPath);

	FVector PrevEndPoint = FVector{ 0,0,0 };
	for (auto i = 0; i < 5; i++)
	{
		ESplineUnit WaveType;

		switch (i % 3)
		{
		case 0:
			WaveType = ESplineUnit::WAVE_SIN;
			break;
		case 1:
			WaveType = ESplineUnit::WAVE_TRIANGLE;
			break;
		default:
			WaveType = ESplineUnit::WAVE_LINEAR;
			break;
		}

		FSplineUnit SplineUnitTmp = FSplineUnit::GenerateSplineUnit(
			WaveType,
			FVector{ 5000.f,0,5000.f },
			PrevEndPoint,
			FVector{ 0,2000.0f, 0 },
			1.0f,
			500,
			10
		);
		PrevEndPoint = SplineUnitTmp.StartLocation + SplineUnitTmp.Distance;

		SplineUnits.Push(SplineUnitTmp);

	}

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