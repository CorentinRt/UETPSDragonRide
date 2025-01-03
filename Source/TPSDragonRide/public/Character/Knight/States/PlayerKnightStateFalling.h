// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Knight/PlayerKnightState.h"
#include "PlayerKnightStateFalling.generated.h"

/**
 * 
 */
UCLASS()
class TPSDRAGONRIDE_API UPlayerKnightStateFalling : public UPlayerKnightState
{
	GENERATED_BODY()

public:
	virtual EPlayerKnightStateID GetStateID() const override;

	virtual void StateInit(UPlayerKnightStateMachine* InStateMachine);
	
	virtual void StateEnter(EPlayerKnightStateID PreviousState);

	virtual void StateEnter(EPlayerKnightStateID PreviousState, int SubStateID);
	
	virtual void StateExit(EPlayerKnightStateID NextState);
	
	virtual void StateTick(float DeltaTime);

	
private:
	void CheckStillFalling();
	
};
