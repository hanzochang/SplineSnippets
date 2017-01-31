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

	// ���C�t�T�C�N�������p�p�����[�^�Q: WIP
	int32 DisplayableSplineUnitLimit = 4; //�O��ɓ����\���\��SplineUnit�̐�
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

    // DebugGrid�A�T�C���n
	UPROPERTY(VisibleAnyWhere)
	TArray<FSplineUnit> SplineUnits;
	
    // DebugGrid�A�T�C���n
	UPROPERTY(EditAnywhere, Category = General)
    TSubclassOf<class AActor> WhatToSpawn;

	/*
	* Spline�ʒu�擾�n 
	*/

	// �C�ӂ̋����ɂ�����Spline��Location���擾
	UFUNCTION(BlueprintCallable, Category=General)
	FVector GetCurrentLocationAlongSpline(float distance);

	// �C�ӂ̋����ɂ�����Spline��Rotate���擾
	UFUNCTION(BlueprintCallable, Category=General)
	FRotator GetCurrentRotationAlongSpline(float distance);

	// �C�ӂ̋����ɂ�����Spline��Direction���擾
	UFUNCTION(BlueprintCallable, Category=General)
	FVector GetCurrentDirectionAlongSpline(float distance);

	//~ Begin ASplineActor Interface.
	UFUNCTION(BlueprintCallable, Category=General)
	void ParseJsonAndAssignSplineUnits(FString Path);

private:
	// (WIP, �ʃ��W���[�����\��)SplinePoints�̏����l���擾
	TArray<FSplineUnit> GenerateInitialSplineUnits(int TestNum, ESplineUnit WaveType);

	// �f�o�b�O�pBluePrint(StaticMesh)��ǂݍ���
	void ASplineActor::LoadDebugGrid();

	// SplinePoint���Ƃ�Debug�pBluePrint���A�T�C��
	void SetDebugGridsEachSplinePoints(int PointNum);
};
