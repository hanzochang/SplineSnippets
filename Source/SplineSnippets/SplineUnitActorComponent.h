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
	// Distance内におけるPointsの配置方法
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    ESplineUnit WaveType;

	// SplineUnitが表現する距離と方向
	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
	FVector Distance;
    
	// カメラがDistance内を通過すべき時間
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    float Msec;
    
	// WaveTypeがLinear以外の場合の最大横幅
    UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Struct")
    float MaxWidth;
    
	// WaveTypeがLinear以外の場合の最小横幅
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
