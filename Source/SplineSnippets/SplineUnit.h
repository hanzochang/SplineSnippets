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
	// Distance���ɂ�����Points�̔z�u���@
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    ESplineUnit WaveType;

	// SplineUnit���\�����鋗���ƕ���
	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
	FVector Distance;
    
	// �J������Distance����ʉ߂��ׂ�����
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    float Msec;
    
	// WaveType��Linear�ȊO�̏ꍇ�̋Ȑ����_�̃x�N�g��
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    FVector VertexVector;

	// WaveType��Linear�ȊO�̏ꍇ��Wave�̐܂�Ԃ���
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    float WaveCycleCount;
    
	// WaveType��Linear�ȊO�̏ꍇ�̍ŏ�����
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    FVector MinWidth;
    
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    int32 Density;

public:
	FString ToDebugString();
	FVector BetweenPoints();

	void DeriveSplinePointsAddTo(TArray<FVector> &Points);

private:
	// linear�J�[�u�ł�SplinePoints���������{
	void DeriveWaveLinearPoints(TArray<FVector> &Points);

	// Sin�J�[�u�ł�SplinePoints���������{
	void DeriveWaveSinPoints(TArray<FVector> &Points);

	// Triangle�J�[�u�ł�SplinePoints���������{
	void DeriveWaveTrianglePoints(TArray<FVector> &Points);
};