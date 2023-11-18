// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTVbase.h"

// Sets default values
ACCTVbase::ACCTVbase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

}

// Called when the game starts or when spawned
void ACCTVbase::BeginPlay()
{
	Super::BeginPlay();

	SetSightConfig(visibleRange, chaseRange, visibleFOV);
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

void ACCTVbase::SetSightConfig(float _visibleRange, float _chaseRange, float _visibleFOV)
{
	UAIPerceptionComponent* PerceptionComp = GetComponentByClass<UAIPerceptionComponent>();
	if (PerceptionComp)
	{
		FAISenseID SenseID = UAISense::GetSenseID<UAISense_Sight>();
		UAISenseConfig_Sight* SenseConfig = Cast<UAISenseConfig_Sight>(PerceptionComp->GetSenseConfig(SenseID));
		if (SenseConfig)
		{
			SenseConfig->SightRadius = _visibleRange;
			SenseConfig->LoseSightRadius = _chaseRange;
			SenseConfig->PeripheralVisionAngleDegrees = _visibleFOV;

			SenseConfig->DetectionByAffiliation.bDetectEnemies = true;
			SenseConfig->DetectionByAffiliation.bDetectFriendlies = true;
			SenseConfig->DetectionByAffiliation.bDetectNeutrals = true;

			SenseConfig->SetMaxAge(0.1f);

			PerceptionComp->ConfigureSense(*SenseConfig);
		}
	}
}

