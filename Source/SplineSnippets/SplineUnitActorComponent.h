// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "SplineUnit.h"
#include "SplineUnitActorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPLINESNIPPETS_API USplineUnitActorComponent : public UActorComponent
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
	// Sets default values for this component's properties
	USplineUnitActorComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

public:
	FVector BetweenPoints();

	FString ToDebugString();
};
