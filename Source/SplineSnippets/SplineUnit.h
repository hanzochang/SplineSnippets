// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SplineUnit.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ESplineUnit : uint8
{
    WAVE_LINEAR      UMETA(DisplayName = "Linear"),
    WAVE_SIN         UMETA(DisplayName = "Sin"),
    WAVE_TRIANGLE    UMETA(DisplayName = "Triangle"),
    WAVE_SAWTOOTH    UMETA(DisplayName = "Sawtooth")
};

USTRUCT(BlueprintType)
struct FSplineUnit
{
    GENERATED_BODY()

public:
	// Distance内におけるPointsの配置方法
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    ESplineUnit WaveType;

	// SplineUnitが表現する距離と方向
	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
	FVector Distance;
    
	// カメラがDistance内を通過すべき時間
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    float Msec;
    
	// WaveTypeがLinear以外の場合の曲線頂点のベクトル
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    FVector VertexVector;

	// WaveTypeがLinear以外の場合のWaveの折り返し数
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    float WaveCycleCount;
    
	// WaveTypeがLinear以外の場合の最小横幅
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    FVector MinWidth;
    
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    int32 Density;

public:
	FString ToDebugString();
	FVector BetweenPoints();

	void DeriveSplinePointsAddTo(TArray<FVector> &Points);

private:
	// linearカーブでのSplinePoints生成を実施
	void DeriveWaveLinearPoints(TArray<FVector> &Points);

	// SinカーブでのSplinePoints生成を実施
	void DeriveWaveSinPoints(TArray<FVector> &Points);

	// TriangleカーブでのSplinePoints生成を実施
	void DeriveWaveTrianglePoints(TArray<FVector> &Points);
};