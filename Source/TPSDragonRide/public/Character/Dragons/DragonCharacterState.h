// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DragonCharacterStateID.h"
#include "Components/ActorComponent.h"
#include "DragonCharacterState.generated.h"


class ACharacterController;
class UCharacterSettings;
class ADragonCharacter;
class UDragonCharacterStateMachine;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPSDRAGONRIDE_API UDragonCharacterState : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDragonCharacterState();

	virtual EDragonCharacterStateID GetStateID() const;

	virtual void StateInit(UDragonCharacterStateMachine* InStateMachine);
	
	virtual void StateEnter(EDragonCharacterStateID PreviousState);

	virtual void StateExit(EDragonCharacterStateID NextState);
	
	virtual void StateTick(float DeltaTime);

protected:
	UPROPERTY()
	TObjectPtr<ADragonCharacter> Character;

	UPROPERTY()
	TObjectPtr<ACharacterController> Controller;

	UPROPERTY()
	TObjectPtr<UDragonCharacterStateMachine> StateMachine;

	UPROPERTY()
	const UCharacterSettings* CharacterSettings;
};
