#include "EnemyFSM.h"
#include "enemy.h"

UEnemyFSM::UEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	me = Cast<AEnemy>(GetOwner());
}


void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mstate)
	{
	case EEnemyState::Patrol:
		PatrolState(DeltaTime);
		break;
	case EEnemyState::Alert:
		AlertState(DeltaTime);
		break;
	case EEnemyState::Check:
		CheckState(DeltaTime);
		break;
	case EEnemyState::Chase:
		ChaseState(DeltaTime);
		break;
	case EEnemyState::Return:
		ReturnState(DeltaTime);
		break;
	case EEnemyState::Neutralize:
		NeutralizeState(DeltaTime);
		break;
	}
}
void UEnemyFSM::PatrolState(float DeltaTime)
{
	me->Patrol();
	if (me->playerExposedTime > 0.5f)
	{
		me->ai->StopMovement();
		mstate = EEnemyState::Alert;
		me->SetStateProperty(me->AlertState);
		me->playerExposedTime = 0.0f;
	}
	else if (me->soundHeardTime > 5.0f || me->cctvdetected)
	{
		me->ai->StopMovement();
		mstate = EEnemyState::Check;
		me->checkTarget = me->player->GetActorLocation();
		me->SetStateProperty(me->CheckState);
		me->soundHeardTime = 0.0f;
	}
};

void UEnemyFSM::AlertState(float DeltaTime) {
	if (me->playerExposedTime > 0.75f || me->cctvdetected)
	{
		me->ai->StopMovement();
		mstate = EEnemyState::Chase;
		me->SetStateProperty(me->ChaseState);
		me->playerExposedTime = 0.0f;
	}
	else if (me->soundHeardTime > 2.0f)
	{
		me->ai->StopMovement();
		mstate = EEnemyState::Check;
		me->checkTarget = me->player->GetActorLocation();
		me->SetStateProperty(me->CheckState);
		me->soundHeardTime = 0.0f;
	}
	else if (me->playerUnExposedTime > 5.0f)
	{
		me->ai->StopMovement();
		mstate = EEnemyState::Return;
		me->SetStateProperty(me->ReturnState);
		me->playerUnExposedTime = 0.0f;
	}
};

void UEnemyFSM::CheckState(float DeltaTime) {
	me->ai->MoveToLocation(me->checkTarget);

	if (me->playerExposedTime > 0.75f || me->cctvdetected)
	{
		me->ai->StopMovement();
		mstate = EEnemyState::Chase;
		me->SetStateProperty(me->ChaseState);
		me->playerExposedTime = 0.0f;
	}
	else if ((me->checkTarget - me->GetActorLocation()).Length() < 50.0f)
	{
		mstate = EEnemyState::Alert;
		me->SetStateProperty(me->AlertState);
		me->playerExposedTime = 0.0f;
		me->ai->StopMovement();
	}
};

void UEnemyFSM::ChaseState(float DeltaTime)
{
	me->ai->MoveToLocation(me->player->GetActorLocation());

	if (me->playerUnExposedTime>1.0f && !me->cctvdetected)
	{
		me->ai->StopMovement();
		mstate = EEnemyState::Return;
		me->SetStateProperty(me->ReturnState);
		me->playerUnExposedTime = 0.0f;
	}
};

void UEnemyFSM::AttackState(float DeltaTime) {};
void UEnemyFSM::ReturnState(float DeltaTime)
{
	FVector minDirection = FVector(1000000, 0, 0);
	int minIndex = 0;
	if(me->locations.Num() != 0)
	{
		for (int i = 0; i < me->locations.Num(); i++) {
			FVector locationDirection = (me->locations[i]->GetActorLocation() - me->GetActorLocation());

			if (minDirection.Length() > locationDirection.Length()) {
				minDirection = locationDirection;
				minIndex = i;
			}
		};
		AActor* minLocation = me->locations[minIndex];
		me->ai->MoveToLocation(minLocation->GetActorLocation());
	}
	else
	{
		me->ai->MoveToLocation(me->originPos);
		minDirection = me->originPos - me->GetActorLocation();
	}

	minDirection.Z = 0;
	

	if (me->playerExposedTime > 1.0f || me->cctvdetected)
	{
		me->ai->StopMovement();
		mstate = EEnemyState::Chase;
		me->SetStateProperty(me->ChaseState);

		me->playerExposedTime = 0.0f;
	}
	else if (minDirection.Length() < 50.0f)
	{
		mstate = EEnemyState::Patrol;
		if (me->locations.Num() != 0)
		{
			me->locationIndex = minIndex;
		}
		me->patrolDelay = 0.0f;
		me->SetStateProperty(me->PatrolState);
		me->ai->StopMovement();
	}
};
void UEnemyFSM::NeutralizeState(float DeltaTime) {};