// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "enemy.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<ACharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacter::StaticClass()));
	me = Cast<Aenemy>(GetOwner());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mstate)
	{
	case EEnemyState::Patrol:
		PatrolState();
		break;
	case EEnemyState::Alert:
		AlertState();
		break;
	case EEnemyState::Check:
		CheckState();
		break;
	case EEnemyState::Chase:
		ChaseState();
		break;
	case EEnemyState::Return:
		ReturnState();
		break;
	case EEnemyState::Neutralize:
		NeutralizeState();
		break;
	}

	FVector dst = player->GetActorLocation();
	FVector dir = dst - me->GetActorLocation();
	if (dir.Length() > me->chaseRange)
	{
		me->IsPlayerVisible = false;
	}

	// ...
}
void UEnemyFSM::PatrolState()
{
	if (me->IsPlayerVisible)
	{
		mstate = EEnemyState::Chase;
	}
};
void UEnemyFSM::AlertState() {};
void UEnemyFSM::CheckState() {};
void UEnemyFSM::ChaseState()
{
	FVector dst = player->GetActorLocation();
	FVector dir = dst - me->GetActorLocation();
	if (!me->IsPlayerVisible)
	{
		mstate = EEnemyState::Return;
	}
	me->AddMovementInput(dir.GetSafeNormal(), me->chaseSpeed);
};
void UEnemyFSM::AttackState() {};
void UEnemyFSM::ReturnState()
{
	FVector dir = me->originPos - me->GetActorLocation();
	me->AddMovementInput(dir.GetSafeNormal(), me->chaseSpeed);
	if ((me->originPos - me->GetActorLocation()).Length() < 1)
	{
		me->SetActorLocationAndRotation(me->originPos, me->originRot);
		mstate = EEnemyState::Patrol;
	}
};
void UEnemyFSM::NeutralizeState() {};

