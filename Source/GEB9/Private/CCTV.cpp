// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTV.h"

// Sets default values
ACCTV::ACCTV()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACCTV::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACCTV::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACCTV::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACCTV::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const {
}

