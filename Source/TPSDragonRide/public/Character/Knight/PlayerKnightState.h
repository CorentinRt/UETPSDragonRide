// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerKnightStateID.h"
#include "UObject/Object.h"
#include "PlayerKnightState.generated.h"

class APlayerKnightCharacter;
class UPlayerKnightStateMachine;
class ACharacterController;
class UCharacterSettings;
/**
 * 
 */
UCLASS(Blueprintable)
class TPSDRAGONRIDE_API UPlayerKnightState : public UObject
{
	GENERATED_BODY()

public:
	virtual EPlayerKnightStateID GetStateID() const;

	virtual void StateInit(UPlayerKnightStateMachine* InStateMachine);
	
	virtual void StateEnter(EPlayerKnightStateID PreviousState);

	virtual void StateEnter(EPlayerKnightStateID PreviousState, int SubStateID);
	
	virtual void StateExit(EPlayerKnightStateID NextState);
	
	virtual void StateTick(float DeltaTime);

protected:
	UPROPERTY()
	TObjectPtr<APlayerKnightCharacter> Character;

	UPROPERTY()
	TObjectPtr<ACharacterController> Controller;

	UPROPERTY()
	TObjectPtr<UPlayerKnightStateMachine> StateMachine;

	UPROPERTY()
	const UCharacterSettings* CharacterSettings;
};
