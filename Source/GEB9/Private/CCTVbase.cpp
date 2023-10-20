// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTVbase.h"

// Sets default values
ACCTVbase::ACCTVbase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACCTVbase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACCTVbase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACCTVbase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACCTVbase::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const {
	OutLocation = GetMesh()->GetSocketLocation("CCTV_socket");
	OutRotation = GetMesh()->GetSocketRotation("CCTV_socket");
}

