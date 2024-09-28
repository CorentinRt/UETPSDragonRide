// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DragonCharacterStateID.h"
#include "UObject/Object.h"
#include "DragonCharacterStateMachine.generated.h"

class UDragonCharacterState;
class ADragonCharacter;
/**
 * 
 */
UCLASS()
class TPSDRAGONRIDE_API UDragonCharacterStateMachine : public UObject
{
	GENERATED_BODY()

public:

	void Init(ADragonCharacter* InCharacter);

	ADragonCharacter* GetCharacter() const;

	UFUNCTION(BlueprintCallable)
	void ChangeState(EDragonCharacterStateID NextStateID);

	UDragonCharacterState* GetState(EDragonCharacterStateID StateID);

	void Tick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	UDragonCharacterState* GetCurrentState() const;

	UFUNCTION()
	EDragonCharacterStateID GetCurrentStateID() const;
	
protected:

	UPROPERTY()
	TObjectPtr<ADragonCharacter> Character;

	UPROPERTY()
	TArray<UDragonCharacterState*> AllStates;

	UPROPERTY()
	EDragonCharacterStateID CurrentStateID;

	UPROPERTY()
	TObjectPtr<UDragonCharacterState> CurrentState;

	void FindStates();

	void InitStates();
};
