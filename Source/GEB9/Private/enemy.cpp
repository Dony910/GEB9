// Fill out your copyright notice in the Description page of Project Settings.


#include "enemy.h"

// Sets default values
Aenemy::Aenemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));

	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));
	IsPlayerVisible = false;

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Load Mesh Fail"))
	}

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = visibleRange;
	SightConfig->LoseSightRadius = chaseRange;
	SightConfig->PeripheralVisionAngleDegrees = visibleFOV;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	SightConfig->SetMaxAge(0.1f);

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
}

// Called when the game starts or when spawned
void Aenemy::BeginPlay()
{
	Super::BeginPlay();
	originPos = GetActorLocation();
	originRot = GetActorRotation();
}

// Called every frame
void Aenemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Aenemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

