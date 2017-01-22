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
	test = true;
	CurrentToSplineUnitNum = 0;

	LoadDebugGrid();
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();

	ParseJsonAndAssignSplineUnits("splinetest.json");

	TArray<FVector> SplinePoints;
	//for (auto SplineUnit : SplineUnits)
	//{
	//	SplineUnit.DeriveSplinePointsAddTo(SplinePoints);
	//};

	SplineUnits[0].DeriveSplinePointsAddTo(SplinePoints);
	MySpline->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::Type::Local);

	// last
	PrevSplineUnitPointStartNum = 0;
	PrevSplineUnitPointEndNum = MySpline->GetNumberOfSplinePoints() - 1;
	CurrentToSplineUnitNum = 0;
	CurrentSplineUnitLength = GetCurrentSplineUnitLength(MySpline, PrevSplineUnitPointStartNum, PrevSplineUnitPointEndNum);
	TotalSplineUnitLength = 0;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::FromInt(CurrentSplineUnitLength));

	for (auto i = 0 ; i < MySpline->GetNumberOfSplinePoints(); i++) {
		SetDebugGridsEachSplinePoints(i);
	}

}

// Called every frame
void ASplineActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	// checklength
}

float ASplineActor::GetCurrentSplineUnitLength(USplineComponent *Spline, int32 PointStartNumber, int32 PointEndNumber)
{
	float LastLength = Spline->GetDistanceAlongSplineAtSplinePoint(PointEndNumber);
    float StartLength = Spline->GetDistanceAlongSplineAtSplinePoint(PointStartNumber);

	return LastLength - StartLength;
}

void ASplineActor::CheckNextSplineUnitsSpawing(float CurrentLength)
{
	if ((CurrentLength > (CurrentSplineUnitLength/2 + TotalSplineUnitLength)) && test)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("hoi"));

		CurrentToSplineUnitNum = CurrentToSplineUnitNum + 1;
		if (CurrentToSplineUnitNum > SplineUnits.Num())
		{ 
			CurrentToSplineUnitNum = 0;
		}
		TArray<FVector> SplinePoints;
		SplineUnits[CurrentToSplineUnitNum].DeriveSplinePointsAddTo(SplinePoints);

		// メソッド生やす
		for (auto SplinePoint : SplinePoints)
		{
			MySpline->AddSplinePoint(SplinePoint, ESplineCoordinateSpace::Type::Local);
		};

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(MySpline->GetNumberOfSplinePoints()));
		for (auto i = PrevSplineUnitPointEndNum + 1 ; i < MySpline->GetNumberOfSplinePoints(); i++) {
			SetDebugGridsEachSplinePoints(i);
		}

		PrevSplineUnitPointStartNum = PrevSplineUnitPointEndNum + 1;
		//PrevSplineUnitPointStartNum = 0;
		PrevSplineUnitPointEndNum = MySpline->GetNumberOfSplinePoints() - 1;
		CurrentSplineUnitLength = GetCurrentSplineUnitLength(MySpline, PrevSplineUnitPointStartNum, PrevSplineUnitPointEndNum);
		TotalSplineUnitLength += CurrentSplineUnitLength;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::FromInt(CurrentSplineUnitLength));
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("dani"));
	}
}


/*
* Private
*/

void ASplineActor::LoadDebugGrid()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> DebugGridClassFinder( TEXT( "Blueprint'/Game/T_DebugGrid.T_DebugGrid'" ) );

	if (DebugGridClassFinder.Succeeded())
	{
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
		MySpline->GetLocationAtSplinePoint(PointNum, ESplineCoordinateSpace::Type::Local), 
		MySpline->GetRotationAtSplinePoint(PointNum, ESplineCoordinateSpace::Type::Local),
		SpawnParams);
}


void ASplineActor::ParseJsonAndAssignSplineUnits(FString Path)
{
	FVector PrevEndPoint = FVector{ 0,0,0 };

	FString FullPath = FPaths::GameContentDir();
	FullPath += Path;
	FString JsonStr;
	FFileHelper::LoadFileToString(JsonStr, *FullPath);

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonStr);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Deserialize"));
		}
		TArray<TSharedPtr<FJsonValue>> objArray = JsonObject->GetArrayField(TEXT("SplineUnits"));

		for (int32 i = 0; i < objArray.Num(); i++)
		{
			TSharedPtr<FJsonValue> value = objArray[i];
			TSharedPtr<FJsonObject> json = value->AsObject();

			FString WaveTypeStr = json->GetStringField(TEXT("WaveType"));
			ESplineUnit WaveType;

			if (WaveTypeStr == "WAVE_LINEAR")
			{
				WaveType = ESplineUnit::WAVE_LINEAR;
			}
			else if (WaveTypeStr == "WAVE_SIN")
			{
				WaveType = ESplineUnit::WAVE_SIN;
			}
			else if (WaveTypeStr == "WAVE_TRIANGLE")
			{
				WaveType = ESplineUnit::WAVE_TRIANGLE;
			}
			else
			{
				WaveType = ESplineUnit::WAVE_LINEAR;
			}

			// TODO Jsonのパラメータ型チェック　Jsonのすべてが正しく存在するか？

			TArray<FString> DistanceJson;
			json->TryGetStringArrayField(TEXT("Distance"), DistanceJson);
			FVector Distance = FVector{ FCString::Atof(*DistanceJson[0]), FCString::Atof(*DistanceJson[1]), FCString::Atof(*DistanceJson[2]) };

			TArray<FString> VertexVectorJson;
			json->TryGetStringArrayField(TEXT("VertexVector"), VertexVectorJson);
			FVector VertexVector = FVector{ FCString::Atof(*VertexVectorJson[0]), FCString::Atof(*VertexVectorJson[1]), FCString::Atof(*VertexVectorJson[2]) };

			float WaveCycleCount = json->GetNumberField(TEXT("WaveCycleCount"));
			int32 Density = json->GetNumberField(TEXT("Density"));
			float Msec = json->GetNumberField(TEXT("Msec"));

			if (GEngine) {
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("json:"));
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, GetSplineUnitEnumAsString(WaveType));
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Distance.ToString());
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, VertexVector.ToString());
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::FromInt(Density));
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::SanitizeFloat(Msec));
			}

			FSplineUnit SplineUnit = FSplineUnit::GenerateSplineUnit(
				WaveType,
				Distance,
				PrevEndPoint,
				VertexVector,
				WaveCycleCount,
				Density,
				Msec
			);

			PrevEndPoint = SplineUnit.StartLocation + SplineUnit.Distance;
			SplineUnits.Push(SplineUnit);
		}
	}
	else {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Not Deserialize"));
		}
	}
}

FVector ASplineActor::GetCurrentLocationAlongSpline(float distance)
{
	return MySpline->GetWorldLocationAtDistanceAlongSpline(distance);
}

FVector ASplineActor::GetCurrentDirectionAlongSpline(float distance)
{
	return MySpline->GetWorldDirectionAtDistanceAlongSpline(distance);
}

FRotator ASplineActor::GetCurrentRotationAlongSpline(float distance)
{
	return MySpline->GetWorldRotationAtDistanceAlongSpline(distance);
}
