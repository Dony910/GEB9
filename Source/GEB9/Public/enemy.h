// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "EnemyFSM.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"

#include "enemy.generated.h"

UCLASS()
class GEB9_API Aenemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Aenemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSMComponent)
	UEnemyFSM* fsm;
	UPROPERTY(VisibleAnywhere, Category = FSM)
	FVector originPos;
	FRotator originRot;

	UPROPERTY(EditAnywhere, Category = FSM)
	float visibleRange = 1000;
	UPROPERTY(EditAnywhere, Category = FSM)
	float chaseRange = 1500;
	UPROPERTY(EditAnywhere, Category = FSM)
	float visibleFOV = 60;
	UPROPERTY(EditAnywhere, Category = FSM)
	float chaseSpeed = 0.5;

	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSM);
	bool IsPlayerVisible;

};
