// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Kismet/GameplayStatics.h>
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8 //크기할당 uint8
{
	Patrol,
	Alert,
	Check,
	Chase,
	Return,
	Neutralize
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GEB9_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM);
	EEnemyState mstate = EEnemyState::Patrol;

	void PatrolState();
	void AlertState();
	void CheckState();
	void ChaseState();
	void AttackState();
	void ReturnState();
	void NeutralizeState();


	float currenTime = 0;

	UPROPERTY(VisibleAnywhere, Category = FSM)
	ACharacter* player;

	UPROPERTY(VisibleAnywhere)
	class Aenemy* me;
};
