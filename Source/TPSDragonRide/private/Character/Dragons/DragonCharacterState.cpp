// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragons/DragonCharacterState.h"

#include "Character/CharacterController.h"
#include "Character/Dragons/DragonCharacter.h"
#include "Character/Dragons/DragonCharacterStateMachine.h"
#include "Character/CharacterSettings.h"

// Sets default values for this component's properties
UDragonCharacterState::UDragonCharacterState()
{
	PrimaryComponentTick.bCanEverTick = false;

}

EDragonCharacterStateID UDragonCharacterState::GetStateID() const
{
	return EDragonCharacterStateID::None;
}

void UDragonCharacterState::StateInit(UDragonCharacterStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;

	if (StateMachine != nullptr)
	{
		Character = StateMachine->GetCharacter();
	}
	
	CharacterSettings = GetDefault<UCharacterSettings>();
}

void UDragonCharacterState::StateEnter(EDragonCharacterStateID PreviousState)
{
	
}

void UDragonCharacterState::StateExit(EDragonCharacterStateID NextState)
{
	
}

void UDragonCharacterState::StateTick(float DeltaTime)
{
	
}



