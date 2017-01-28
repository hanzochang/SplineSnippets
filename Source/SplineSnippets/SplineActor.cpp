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
	DisplayableSplineUnitLimit = 4;

	LoadDebugGrid();
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();

	ParseJsonAndAssignSplineUnits("splinetest.json");

	// まず前面だけ
	TArray<FVector> SplinePoints;
	FVector StartPoint = FVector{ 0,0,0 };
	int32 EdgeSplineUnitStartSplineNum = 0;
	int32 counter = 0;

	for (auto i = 0; i < DisplayableSplineUnitLimit; i++)
	{
		EdgeSplineUnitStartSplineNum = MySpline->GetNumberOfSplinePoints();
		counter += i;
		if (counter >= SplineUnits.Num()) { counter = 0; }

		SplineUnits[counter].DeriveSplinePointsAddTo(SplinePoints, StartPoint);
		MySpline->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::Type::Local);
		StartPoint = MySpline->GetLocationAtSplinePoint(MySpline->GetNumberOfSplinePoints(),
			                                            ESplineCoordinateSpace::Local);

		PrevSplineUnitPointStartNum = EdgeSplineUnitStartSplineNum;
		PrevSplineUnitPointEndNum = MySpline->GetNumberOfSplinePoints() - 1;
		CurrentSplineUnitLength = GetCurrentSplineUnitLength(MySpline,
															 PrevSplineUnitPointStartNum,
															 PrevSplineUnitPointEndNum);

		DisplayableSplineUnitLengths.Push(CurrentSplineUnitLength);
	}

	CurrentToSplineUnitNum = 0;
	TotalSplineUnitLength = 0;

	for (auto i = 0 ; i < MySpline->GetNumberOfSplinePoints(); i++) {
		SetDebugGridsEachSplinePoints(i);
	}

	DisplayableSplineUnitLength = 0;
	for(auto i = 0; i <= CurrentToSplineUnitNum; i++)
	{
		DisplayableSplineUnitLength += DisplayableSplineUnitLengths[i];
	}

}

// Called every frame
void ASplineActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

float ASplineActor::GetCurrentSplineUnitLength(USplineComponent *Spline, int32 PointStartNumber, int32 PointEndNumber)
{
	float LastLength = Spline->GetDistanceAlongSplineAtSplinePoint(PointEndNumber);
    float StartLength = Spline->GetDistanceAlongSplineAtSplinePoint(PointStartNumber);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(PointEndNumber));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::FromInt(PointStartNumber));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(LastLength));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::SanitizeFloat(StartLength));

	return LastLength - StartLength;
}

void ASplineActor::CheckNextSplineUnitsSpawing(float CurrentLength)
{
	if ((CurrentLength > DisplayableSplineUnitLength) && test)
	{

		CurrentToSplineUnitNum = CurrentToSplineUnitNum + 1;
		int32 RenderSplineUnitNum = CurrentToSplineUnitNum;
		if (RenderSplineUnitNum >= SplineUnits.Num())
		{
			RenderSplineUnitNum = CurrentToSplineUnitNum % SplineUnits.Num();
		}

		TArray<FVector> SplinePoints;
		SplineUnits[RenderSplineUnitNum].
			DeriveSplinePointsAddTo(SplinePoints,
				MySpline->GetLocationAtSplinePoint(MySpline->GetNumberOfSplinePoints(), ESplineCoordinateSpace::Local),
				MySpline->GetDirectionAtSplinePoint(MySpline->GetNumberOfSplinePoints(), ESplineCoordinateSpace::Local)
			);


		FVector UnitVector = MySpline->GetDirectionAtDistanceAlongSpline(MySpline->GetSplineLength(), ESplineCoordinateSpace::Local);
		FVector UnitVectorVertical = FRotator{ 90, 0, 0 }.RotateVector(UnitVector);
		UnitVectorVertical = FVector{ 0, 0, UnitVectorVertical.Z };
		FVector Loc = MySpline->GetLocationAtSplinePoint(MySpline->GetNumberOfSplinePoints(), ESplineCoordinateSpace::Local);
		FRotator Rot = MySpline->GetRotationAtSplinePoint(MySpline->GetNumberOfSplinePoints() -1, ESplineCoordinateSpace::Local);

		FVector LastSplinePoint = SplinePoints[SplinePoints.Num() - 1];
		FVector Ve = LastSplinePoint - Loc;
		Ve.Normalize();
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Blue, Rot.ToString());
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, Ve.Rotation().ToString());
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::White, FMath::DegreesToRadians((Ve.Rotation() + Rot)).ToString());
		//PrevRotation += Ve.Rotation();
		PrevRotation += FRotator{ 0,10,0 };
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, FMath::DegreesToRadians(PrevRotation).ToString());
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, PrevRotation.ToString());

		//FQuat quat = FQuat{ UnitVectorVertical, FMath::DegreesToRadians(Ve.Rotation().Yaw + Rot.Yaw) };
		FQuat quat = FQuat{ UnitVectorVertical, FMath::DegreesToRadians(PrevRotation.Yaw) };
		//FQuat quat2;
		//quat2 = FQuat{ UnitVectorVertical, Rot.Pitch };
		for (auto SplinePoint : SplinePoints)
		{
			//あそび
			GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, quat.RotateVector(SplinePoint - Loc).ToString());
			MySpline->AddSplinePoint((quat.RotateVector(SplinePoint - Loc)+Loc), ESplineCoordinateSpace::Type::Local);
			//MySpline->AddSplinePoint(SplinePoint, ESplineCoordinateSpace::Type::Local);
		};

		for (auto i = PrevSplineUnitPointEndNum + 1 ; i < MySpline->GetNumberOfSplinePoints(); i++) {
			SetDebugGridsEachSplinePoints(i);
		}

		PrevSplineUnitPointStartNum = PrevSplineUnitPointEndNum + 1;
		PrevSplineUnitPointEndNum = MySpline->GetNumberOfSplinePoints() - 1;
		CurrentSplineUnitLength = GetCurrentSplineUnitLength(MySpline,
															 PrevSplineUnitPointStartNum,
															 PrevSplineUnitPointEndNum);
		DisplayableSplineUnitLengths.Push(CurrentSplineUnitLength);
		DisplayableSplineUnitLength = 0;
		for(auto i = 0; i < CurrentToSplineUnitNum; i++)
		{
			DisplayableSplineUnitLength += DisplayableSplineUnitLengths[i];
		}
		DisplayableSplineUnitLength += DisplayableSplineUnitLengths[CurrentToSplineUnitNum] / 2;

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
	static ConstructorHelpers::FObjectFinder<UBlueprint> DebugGridClassFinder( TEXT( "Blueprint'/Game/T_DebugGrid2.T_DebugGrid2'" ) );

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


// JSONパーサー / 格納 別クラスにする
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
