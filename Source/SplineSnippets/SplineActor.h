// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SplineUnit.h"
#include "Components/SplineComponent.h"
#include "SplineActor.generated.h"


UCLASS(Blueprintable)
class SPLINESNIPPETS_API ASplineActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASplineActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnyWhere)
	USplineComponent* MySpline;

	UPROPERTY(VisibleAnyWhere)
	UStaticMeshComponent* SM;

	// ライフサイクル生成用パラメータ群: WIP
	int32 DisplayableSplineUnitLimit = 4; //前後に同時表示可能なSplineUnitの数
	TArray<float> DisplayableSplineUnitLengths;
	float DisplayableSplineUnitLength;

	int32 CurrentToSplineUnitNum = 0;
	float CurrentSplineUnitLength;
	float TotalSplineUnitLength;
	int32 PrevSplineUnitPointStartNum;
	int32 PrevSplineUnitPointEndNum;
	float GetCurrentSplineUnitLength(USplineComponent *Spline, int32 PointStartNumber, int32 PointEndNumber);

	FRotator PrevRotation = FRotator{ 0,0,0 }; // radian

	UFUNCTION(BlueprintCallable, Category=General)
	void CheckNextSplineUnitsSpawing(float CurrentLength);

    // DebugGridアサイン系
	UPROPERTY(VisibleAnyWhere)
	TArray<FSplineUnit> SplineUnits;
	
    // DebugGridアサイン系
	UPROPERTY(EditAnywhere, Category = General)
    TSubclassOf<class AActor> WhatToSpawn;

	/*
	* Spline位置取得系 
	*/

	// 任意の距離におけるSplineのLocationを取得
	UFUNCTION(BlueprintCallable, Category=General)
	FVector GetCurrentLocationAlongSpline(float distance);

	// 任意の距離におけるSplineのRotateを取得
	UFUNCTION(BlueprintCallable, Category=General)
	FRotator GetCurrentRotationAlongSpline(float distance);

	// 任意の距離におけるSplineのDirectionを取得
	UFUNCTION(BlueprintCallable, Category=General)
	FVector GetCurrentDirectionAlongSpline(float distance);

	//~ Begin ASplineActor Interface.
	UFUNCTION(BlueprintCallable, Category=General)
	void ParseJsonAndAssignSplineUnits(FString Path);

private:
	// (WIP, 別モジュール化予定)SplinePointsの初期値を取得
	TArray<FSplineUnit> GenerateInitialSplineUnits(int TestNum, ESplineUnit WaveType);

	// デバッグ用BluePrint(StaticMesh)を読み込む
	void ASplineActor::LoadDebugGrid();

	// SplinePointごとにDebug用BluePrintをアサイン
	void SetDebugGridsEachSplinePoints(int PointNum);
};
