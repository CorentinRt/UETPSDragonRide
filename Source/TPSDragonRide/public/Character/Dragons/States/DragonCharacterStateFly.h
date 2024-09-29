// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Dragons/DragonCharacterState.h"
#include "DragonCharacterStateFly.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPSDRAGONRIDE_API UDragonCharacterStateFly : public UDragonCharacterState
{
	GENERATED_BODY()

public:

	virtual EDragonCharacterStateID GetStateID() const override;
	
	virtual void StateInit(UDragonCharacterStateMachine* InStateMachine) override;

	virtual void StateEnter(EDragonCharacterStateID PreviousState) override;

	virtual void StateExit(EDragonCharacterStateID NextState) override;

	virtual void StateTick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UAnimMontage* FlyMontage;

	UPROPERTY(EditAnywhere)
	float FlySpeed = 10000.f;


#pragma region Fly Behavior

	UPROPERTY()
	float CurrentFlySpeed;
	UPROPERTY(EditAnywhere)
	float MaxFlySpeed = 12000.f;
	UPROPERTY(EditAnywhere)
	float FlySpeedAcceleration = 1.f;

	UPROPERTY()
	float CurrentGravityApplied;
	UPROPERTY(EditAnywhere)
	float MaxGravityApplied = 6000.f;
	UPROPERTY(EditAnywhere)
	float GravityAppliedAcceleration = 0.5f;

	UFUNCTION()
	void HandleFlyRotation(float DeltaTime);

	UFUNCTION()
	void HandleFly(float DeltaTime);
	
#pragma endregion 
	
	
	UFUNCTION()
	void OnReceiveInputDive(float DiveValue);
};
