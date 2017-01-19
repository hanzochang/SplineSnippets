// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineSnippets.h"
#include "SplinePawn.h"


// Sets default values
ASplinePawn::ASplinePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASplinePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASplinePawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ASplinePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

