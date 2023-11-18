// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"

#include "CCTVbase.generated.h"

UCLASS()
class GEB9_API ACCTVbase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACCTVbase();
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sight)
	float visibleRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sight)
	float chaseRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sight)
	float visibleFOV;

	UAISenseConfig_Sight* SightConfig;
	void SetSightConfig(float visibleRange, float chaseRange, float visibleFOV);
};
