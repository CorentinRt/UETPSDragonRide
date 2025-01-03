// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerKnightStateID.h"
#include "UObject/Object.h"
#include "PlayerKnightStateMachine.generated.h"

class UPlayerKnightState;
class APlayerKnightCharacter;
/**
 * 
 */
UCLASS()
class TPSDRAGONRIDE_API UPlayerKnightStateMachine : public UObject
{
	GENERATED_BODY()

public:

	void Init(APlayerKnightCharacter* InCharacter);

	APlayerKnightCharacter* GetCharacter() const;

	UFUNCTION(BlueprintCallable)
	void ChangeState(EPlayerKnightStateID NextStateID, int InSubstateID = 0);

	UPlayerKnightState* GetState(EPlayerKnightStateID StateID);

	void Tick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	UPlayerKnightState* GetCurrentState() const;

	UFUNCTION()
	EPlayerKnightStateID GetCurrentStateID() const;
	
protected:

	UPROPERTY()
	TObjectPtr<APlayerKnightCharacter> Character;

	UPROPERTY()
	TArray<UPlayerKnightState*> AllStates;

	UPROPERTY()
	EPlayerKnightStateID CurrentStateID;

	UPROPERTY()
	TObjectPtr<UPlayerKnightState> CurrentState;
	

	void CreateObjectStates();
	
	void InitStates();
};
