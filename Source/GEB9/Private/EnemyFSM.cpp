#include "EnemyFSM.h"
#include "enemy.h"

UEnemyFSM::UEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	me = Cast<Aenemy>(GetOwner());
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
	if (me->playerExposedTime > 2.0f)
	{
		mstate = EEnemyState::Chase;
		me->playerExposedTime = 0.0f;
	}
};
void UEnemyFSM::AlertState(float DeltaTime) {};
void UEnemyFSM::CheckState(float DeltaTime) {};
void UEnemyFSM::ChaseState(float DeltaTime)
{
	FRotator rot = me->GetPlayerDir().Rotation();
	rot.Pitch = 0;
	rot.Roll = 0;
	me->SetActorRotation(rot);
	me->AddMovementInput(me->GetPlayerDir().GetSafeNormal(), me->chaseSpeed* DeltaTime);

	if (me->playerUnExposedTime>1.0f)
	{
		mstate = EEnemyState::Return;

		me->playerUnExposedTime = 0.0f;
	}
};

void UEnemyFSM::AttackState(float DeltaTime) {};
void UEnemyFSM::ReturnState(float DeltaTime)
{
	FVector dir = me->originPos - me->GetActorLocation();
	FRotator rot = dir.Rotation();
	rot.Pitch = 0;
	rot.Roll = 0;
	me->SetActorRotation(rot);
	me->AddMovementInput(dir.GetSafeNormal(), me->chaseSpeed* DeltaTime);

	if ((me->originPos - me->GetActorLocation()).Length() < 1)
	{
		mstate = EEnemyState::Patrol;

		me->SetActorLocationAndRotation(me->originPos, me->originRot);
	}
	else if (me->playerExposedTime > 1.0f)
	{
		mstate = EEnemyState::Chase;

		me->playerExposedTime = 0.0f;
	}
};
void UEnemyFSM::NeutralizeState(float DeltaTime) {};