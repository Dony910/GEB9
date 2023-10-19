#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

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
	float visibleRange=500;
	UPROPERTY(EditAnywhere, Category = Enemy)
	float chaseRange=750;
	UPROPERTY(EditAnywhere, Category = Enemy)
	float visibleFOV=40;
	UPROPERTY(EditAnywhere, Category = Enemy)
	float hearingRange=750;
	UPROPERTY(EditAnywhere, Category = Enemy)
	float chaseSpeed=400;
};

UCLASS()
class GEB9_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(BlueprintReadOnly);
	UEnemyFSM* fsm;
	UPROPERTY(VisibleAnywhere, Category = Enemy)
	FVector originPos;
	UPROPERTY(VisibleAnywhere, Category = Enemy)
	FRotator originRot;
	UPROPERTY(VisibleAnywhere, Category = Enemy)
	FVector checkTarget;

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
	FStateProperty AlertState;
	UPROPERTY(EditAnywhere, Category = Enemy)
	FStateProperty CheckState;
	UPROPERTY(EditAnywhere, Category = Enemy)
	FStateProperty ReturnState;
	UPROPERTY(EditAnywhere, Category = Enemy)
	FStateProperty ChaseState;
	UPROPERTY(EditAnywhere, Category = Enemy)
	FStateProperty efficeiencyState;

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
	UPROPERTY(BlueprintReadWrite, Category = Enemy)
	bool IsHeard;
	UPROPERTY(BlueprintReadWrite, Category = Enemy)
	float soundHeardTime;
	UPROPERTY(BlueprintReadWrite, Category = Enemy)
	float soundUnHeardTime;

	UPROPERTY(EditAnywhere, Category = PahtLocations)
	TArray<AActor*> locations;

	FVector GetPlayerDir();

	void SetStateProperty(FStateProperty stateproperty);

	void SetSightConfig(float visibleRange, float chaseRange, float visibleFOV);
	void SetSightConfig();

	void SetHearingConfig(float HearingRange);
	void SetHearingConfig();

	void lightout();
	void lighton();

	void Turn();

	void Patrol();
};
