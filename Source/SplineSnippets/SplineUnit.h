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
    
	// WaveType��Linear�ȊO�̏ꍇ�̍ő剡��
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    float MaxWidth;
    
	// WaveType��Linear�ȊO�̏ꍇ�̍ŏ�����
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    float MinWidth;
    
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    int32 Density;

public:
	FString ToDebugString();
	FVector BetweenPoints();
};