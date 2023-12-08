#include "enemy.h"

AEnemy::AEnemy()
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

	locationIndex = 0;

	patrolTimer = 0;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	originPos = GetActorLocation();
	originRot = GetActorRotation();


	player = Cast<ACharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacter::StaticClass()));
	ai = Cast<AAIController>(GetController());
	enemyMovement = GetCharacterMovement();

	SetStateProperty(PatrolState);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (enemyMovement->Velocity.Length() != 0) Turn();
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FVector AEnemy::GetPlayerDir()
{
	FVector dst = player->GetActorLocation();
	return dst - GetActorLocation();
}

void AEnemy::SetStateProperty(FStateProperty stateproperty) {
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

void AEnemy::SetSightConfig(float _visibleRange, float _chaseRange, float _visibleFOV)
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
			SenseConfig->AutoSuccessRangeFromLastSeenLocation = 1000;
			SenseConfig->PointOfViewBackwardOffset = 30;

			SenseConfig->DetectionByAffiliation.bDetectEnemies = true;
			SenseConfig->DetectionByAffiliation.bDetectFriendlies = true;
			SenseConfig->DetectionByAffiliation.bDetectNeutrals = true;

			SenseConfig->SetMaxAge(0.1f);

			PerceptionComp->ConfigureSense(*SenseConfig);
		}
	}
}

void AEnemy::SetSightConfig()
{
	SetSightConfig(visibleRange, chaseRange, visibleFOV);
}

void AEnemy::SetHearingConfig(float _hearingRange)
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

			SenseConfig->SetMaxAge(0.01f);

			PerceptionComp->ConfigureSense(*SenseConfig);
		}
	}
}

void AEnemy::SetHearingConfig()
{
	SetHearingConfig(hearingRange);
}

void AEnemy::lightout() {
	efficeiencyState.visibleRange=0.5;
	efficeiencyState.chaseRange =0.5;
	efficeiencyState.visibleFOV=1;
	efficeiencyState.hearingRange=1.2;
	efficeiencyState.chaseSpeed=1;
}

void AEnemy::lighton() {
	efficeiencyState.visibleRange = 1;
	efficeiencyState.chaseRange = 1;
	efficeiencyState.visibleFOV = 1;
	efficeiencyState.hearingRange = 1;
	efficeiencyState.chaseSpeed = 1;
}

void AEnemy::Turn() {
	FRotator rot = enemyMovement->Velocity.Rotation();
	rot.Pitch = 0;
	rot.Roll = 0;
	SetActorRotation(FQuat::Slerp(RootComponent->GetComponentRotation().Quaternion(), rot.Quaternion(), 0.3f));
	//SetActorRotation(rot);
}

void AEnemy::Patrol(float DeltaTime) {
	if (!locations.IsEmpty())
	{
		ai->MoveToLocation(locations[locationIndex]->GetActorLocation());
		FVector targetDirection = GetActorLocation() - locations[locationIndex]->GetActorLocation();
		targetDirection.Z = 0;
		GEngine->AddOnScreenDebugMessage(10, 0.1f, FColor::Yellow, FString::SanitizeFloat(targetDirection.Length()));
		if (targetDirection.Length() < 50.0f) {
			patrolTimer += DeltaTime;
			GEngine->AddOnScreenDebugMessage(13, 0.1f, FColor::Red, FString::SanitizeFloat(patrolTimer));
			if (patrolTimer >= patrolDelay) {
				locationIndex++;
				patrolTimer = 0.0f;
				if (locationIndex >= locations.Num())
					locationIndex = 0;
			}
		}
	}
	else 
	{
		SetActorLocationAndRotation(originPos, originRot);
	}
}


void AEnemy::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const {
	OutLocation = GetMesh()->GetSocketLocation("headsocket");
	OutRotation = GetMesh()->GetSocketRotation("headsocket");
}