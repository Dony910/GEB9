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
	if (me->playerExposedTime > 0.5f)
	{
		mstate = EEnemyState::Alert;
		me->SetStateProperty(me->AlertState);
		me->playerExposedTime = 0.0f;
	}
	else if (me->soundHeardTime > 5.0f)
	{
		mstate = EEnemyState::Check;
		me->checkTarget = me->player->GetActorLocation();
		me->SetStateProperty(me->CheckState);
		me->soundHeardTime = 0.0f;
	}
};
void UEnemyFSM::AlertState(float DeltaTime) {
	if (me->playerExposedTime > 0.75f)
	{
		mstate = EEnemyState::Chase;
		me->SetStateProperty(me->ChaseState);
		me->playerExposedTime = 0.0f;
	}
	else if (me->soundHeardTime > 2.0f)
	{
		mstate = EEnemyState::Check;
		me->checkTarget = me->player->GetActorLocation();
		me->SetStateProperty(me->CheckState);
		me->soundHeardTime = 0.0f;
	}
	else if (me->playerUnExposedTime > 5.0f)
	{
		mstate = EEnemyState::Return;
		me->SetStateProperty(me->ReturnState);
		me->playerUnExposedTime = 0.0f;
	}
};
void UEnemyFSM::CheckState(float DeltaTime) {
	me->ai->MoveToLocation(me->checkTarget);

	if (me->playerExposedTime > 0.75f)
	{
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

	if (me->playerUnExposedTime>1.0f)
	{
		mstate = EEnemyState::Return;
		me->SetStateProperty(me->ReturnState);

		me->playerUnExposedTime = 0.0f;
	}
};

void UEnemyFSM::AttackState(float DeltaTime) {};
void UEnemyFSM::ReturnState(float DeltaTime)
{
	FVector dir = me->originPos - me->GetActorLocation();

	me->ai->MoveToLocation(me->originPos);

	if (me->playerExposedTime > 1.0f)
	{
		mstate = EEnemyState::Chase;
		me->SetStateProperty(me->ChaseState);

		me->playerExposedTime = 0.0f;
	}
	else if ((me->originPos - me->GetActorLocation()).Length() < 50.0f)
	{
		mstate = EEnemyState::Patrol;
		me->SetStateProperty(me->PatrolState);
		me->ai->StopMovement();
		me->SetActorLocation(me->originPos);
		me->SetActorRotation(me->originRot);
	}
};
void UEnemyFSM::NeutralizeState(float DeltaTime) {};