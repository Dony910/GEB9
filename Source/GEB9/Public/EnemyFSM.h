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
	UEnemyFSM();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM);
	EEnemyState mstate = EEnemyState::Patrol;

	void PatrolState(float DeltaTime);
	void AlertState(float DeltaTime);
	void CheckState(float DeltaTime);
	void ChaseState(float DeltaTime);
	void AttackState(float DeltaTime);
	void ReturnState(float DeltaTime);
	void NeutralizeState(float DeltaTime);

	UPROPERTY(VisibleAnywhere)
	class AEnemy* me;
};
