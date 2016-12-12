// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineSnippets.h"
#include "SplineActor.h"


// Sets default values
ASplineActor::ASplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SplineUnits = GenerateInitialSplineUnits(10);

	TArray<FVector> Points;
	for (auto i = 0; i < 5; i++) {
		FVector TmpVector = FVector(i * 100);
		Points.Push(TmpVector);
	}

	SM = CreateDefaultSubobject<UStaticMeshComponent>(FName("SM"));
	SetRootComponent(SM);

    MySpline = CreateDefaultSubobject<USplineComponent>(FName("MySpline"));
	MySpline->SetupAttachment(SM);

	MySpline->SetSplinePoints(Points, ESplineCoordinateSpace::Type::Local);
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASplineActor::Tick( float DeltaTime )
{
	//UE_LOG(LogTemp, Warning, TEXT("output : %s"), L"ログの内容");
	Super::Tick( DeltaTime );

}

TArray<FSplineUnit> ASplineActor::GenerateInitialSplineUnits(int TestNum)
{
	TArray<FSplineUnit> SplineUnitsTmp;

	for (auto i = 0; i < TestNum; i++) {
		FSplineUnit SplineUnitTmp;
		
		SplineUnitTmp.WaveType = ESplineUnit::WAVE_LINEAR;
		SplineUnitTmp.Msec = 10.0f;
		SplineUnitTmp.MaxWidth = 0;
		SplineUnitTmp.MinWidth = 0;
		SplineUnitTmp.Density = 10;

		SplineUnitsTmp.Push(SplineUnitTmp);

		FVector UnitVector = FVector(1.0f);
		SplineUnitTmp.Distance = UnitVector * i;
		SplineUnitsTmp.Push(SplineUnitTmp);
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor(255, 255, 255, 255), FSplineUnit::ToString(SplineUnitTmp));

		//UE_LOG(LogTemp, Warning, TEXT("output : %s"), L"ログの内容");
		//OutputDebugString("テスト%d", SplineUnitTmp); //ここから
		//	FMath::RandRange(1.0f, 2.0f)(float);
		//GEngine->AddOnScreenDebugMessage((uint64)-1, 10.f, FLinearColor::Red.ToFColor(true), 'waa');
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor(255, 255, 255, 255), "TstMan");
    }

	return SplineUnitsTmp;
}