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
	UPROPERTY(VisibleAnyWhere)
	USplineComponent* MySpline;

	UPROPERTY(VisibleAnyWhere)
	UStaticMeshComponent* SM;

	float DeltaTimeSum;

	// Sets default values for this actor's properties
	ASplineActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnyWhere)
	TArray<FSplineUnit> SplineUnits;
	
	UPROPERTY(EditAnywhere, Category = General)
    TSubclassOf<class AActor> WhatToSpawn;

	void ASplineActor::ForceMove(AActor *Actor);

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
