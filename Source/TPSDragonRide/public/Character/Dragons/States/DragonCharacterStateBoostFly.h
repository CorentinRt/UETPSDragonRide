// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Dragons/DragonCharacterState.h"
#include "DragonCharacterStateBoostFly.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPSDRAGONRIDE_API UDragonCharacterStateBoostFly : public UDragonCharacterState
{
	GENERATED_BODY()

public:

	virtual EDragonCharacterStateID GetStateID() const override;
	
	virtual void StateInit(UDragonCharacterStateMachine* InStateMachine) override;

	virtual void StateEnter(EDragonCharacterStateID PreviousState) override;

	virtual void StateExit(EDragonCharacterStateID NextState) override;

	virtual void StateTick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UAnimMontage* BoostFlyMontage;


	UPROPERTY(EditAnywhere)
	float MaxBoostFlySpeed = 16000.f;
	
	UPROPERTY(EditAnywhere)
	float BoostFlyDuration = 1.f;

	UPROPERTY(EditAnywhere)
	float BoostFlyAcceleration = 4.f;
	
	UPROPERTY()
	float CurrentBoostFlyDuration;

	UFUNCTION()
	void HandleBoostFlyDuration(float DeltaTime);

	UFUNCTION()
	void HandleBoostFlyVelocity(float DeltaTime);

	UFUNCTION()
	void HandleBoostFlyRotation(float DeltaTime);
};
