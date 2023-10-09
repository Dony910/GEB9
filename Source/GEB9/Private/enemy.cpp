#include "enemy.h"

Aenemy::Aenemy()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Load Mesh Fail"))
	}

	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->ConfigureSense(*HearingConfig);

	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	SetSightConfig();
	SetHearingConfig();

	playerExposedTime = 0;
	playerUnExposedTime = 0;
	IsPlayerVisible = false;
}

void Aenemy::BeginPlay()
{
	Super::BeginPlay();
	originPos = GetActorLocation();
	originRot = GetActorRotation();


	player = Cast<ACharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacter::StaticClass()));
	ai = Cast<AAIController>(GetController());
	enemyMovement = GetCharacterMovement();

	SetStateProperty(PatrolState);
}

void Aenemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void Aenemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FVector Aenemy::GetPlayerDir()
{
	FVector dst = player->GetActorLocation();
	return dst - GetActorLocation();
}

void Aenemy::SetStateProperty(FStateProperty stateproperty) {
	visibleRange = stateproperty.visibleRange*efficeiencyState.visibleRange;
	visibleFOV = stateproperty.visibleFOV * efficeiencyState.visibleFOV;
	chaseRange = stateproperty.chaseRange * efficeiencyState.chaseRange;
	hearingRange = stateproperty.hearingRange * efficeiencyState.hearingRange;
	chaseSpeed = stateproperty.chaseSpeed * efficeiencyState.chaseSpeed;
	if (enemyMovement) {
		enemyMovement->MaxWalkSpeed = chaseSpeed;
	}
	SetSightConfig();
	SetHearingConfig();
}

void Aenemy::SetSightConfig(float _visibleRange, float _chaseRange, float _visibleFOV)
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

void Aenemy::SetSightConfig()
{
	SetSightConfig(visibleRange, chaseRange, visibleFOV);
}

void Aenemy::SetHearingConfig(float _hearingRange)
{
	UAIPerceptionComponent* PerceptionComp = GetComponentByClass<UAIPerceptionComponent>();
	if (PerceptionComp)
	{
		FAISenseID SenseID = UAISense::GetSenseID<UAISense_Hearing>();
		UAISenseConfig_Hearing* SenseConfig = Cast<UAISenseConfig_Hearing>(PerceptionComp->GetSenseConfig(SenseID));
		if (SenseConfig)
		{
			SenseConfig->HearingRange = _hearingRange;

			SenseConfig->DetectionByAffiliation.bDetectEnemies = true;
			SenseConfig->DetectionByAffiliation.bDetectFriendlies = true;
			SenseConfig->DetectionByAffiliation.bDetectNeutrals = true;

			SenseConfig->SetMaxAge(0.1f);

			PerceptionComp->ConfigureSense(*SenseConfig);
		}
	}
}

void Aenemy::SetHearingConfig()
{
	SetHearingConfig(hearingRange);
}

void Aenemy::lightout() {
	efficeiencyState.visibleRange=0.5;
	efficeiencyState.chaseRange =0.5;
	efficeiencyState.visibleFOV=1;
	efficeiencyState.hearingRange=1.2;
	efficeiencyState.chaseSpeed=1;
}

void Aenemy::lighton() {
	efficeiencyState.visibleRange = 1;
	efficeiencyState.chaseRange = 1;
	efficeiencyState.visibleFOV = 1;
	efficeiencyState.hearingRange = 1;
	efficeiencyState.chaseSpeed = 1;
}