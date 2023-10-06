#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "EnemyFSM.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

#include "enemy.generated.h"
USTRUCT()
struct FStateProperty {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Enemy)
	float visibleRange;
	UPROPERTY(EditAnywhere, Category = Enemy)
	float chaseRange;
	UPROPERTY(EditAnywhere, Category = Enemy)
	float visibleFOV;
	UPROPERTY(EditAnywhere, Category = Enemy)
	float hearingRange;
	UPROPERTY(EditAnywhere, Category = Enemy)
	float chaseSpeed;
};

UCLASS()
class GEB9_API Aenemy : public ACharacter
{
	GENERATED_BODY()

public:
	Aenemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(BlueprintReadOnly);
	UEnemyFSM* fsm;
	FVector originPos;
	FRotator originRot;

	UPROPERTY(EditAnywhere, Category = Enemy)
	float visibleRange;
	UPROPERTY(EditAnywhere, Category = Enemy)
	float chaseRange;
	UPROPERTY(EditAnywhere, Category = Enemy)
	float visibleFOV;
	UPROPERTY(EditAnywhere, Category = Enemy)
	float hearingRange;
	UPROPERTY(EditAnywhere, Category = Enemy)
	float chaseSpeed;

	UPROPERTY(EditAnywhere, Category = Enemy)
	FStateProperty PatrolState;
	UPROPERTY(EditAnywhere, Category = Enemy)
	FStateProperty ReturnState;
	UPROPERTY(EditAnywhere, Category = Enemy)
	FStateProperty ChaseState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enemy)
	UAIPerceptionComponent* AIPerception;
	AAIController* ai;

	UAISenseConfig_Sight* SightConfig;
	UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY(VisibleAnywhere, Category = FSM)
	ACharacter* player;
	UCharacterMovementComponent* enemyMovement;

	UPROPERTY(BlueprintReadWrite, Category = Enemy)
	bool IsPlayerVisible;
	UPROPERTY(BlueprintReadWrite, Category = Enemy)
	float playerExposedTime;
	UPROPERTY(BlueprintReadWrite, Category = Enemy)
	float playerUnExposedTime;

	FVector GetPlayerDir();

	void SetStateProperty(FStateProperty stateproperty);

	void SetSightConfig(float visibleRange, float chaseRange, float visibleFOV);
	void SetSightConfig();

	void SetHearingConfig(float HearingRange);
	void SetHearingConfig();
};
