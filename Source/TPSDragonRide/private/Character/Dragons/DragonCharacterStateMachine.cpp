// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragons/DragonCharacterStateMachine.h"

#include "Character/Dragons/DragonCharacter.h"
#include "Character/Dragons/DragonCharacterState.h"

void UDragonCharacterStateMachine::Init(ADragonCharacter* InCharacter)
{
	Character = InCharacter;

	FindStates();
	InitStates();

	ChangeState(EDragonCharacterStateID::Idle);
}

ADragonCharacter* UDragonCharacterStateMachine::GetCharacter() const
{
	return Character;
}

void UDragonCharacterStateMachine::ChangeState(EDragonCharacterStateID NextStateID)
{
	UDragonCharacterState* NextState = GetState(NextStateID);

	if (NextState == nullptr) return;
	
	if (CurrentState != nullptr)
	{
		CurrentState->StateExit(NextStateID);
	}

	EDragonCharacterStateID PreviousStateID = CurrentStateID;
	CurrentStateID = NextStateID;
	CurrentState = NextState;
	
	if (CurrentState != nullptr)
	{
		CurrentState->StateEnter(PreviousStateID);
	}
}

UDragonCharacterState* UDragonCharacterStateMachine::GetState(EDragonCharacterStateID StateID)
{
	for (UDragonCharacterState* State : AllStates)
	{
		if (StateID == State->GetStateID())
		{
			return State;
		}
	}

	return nullptr;
}

void UDragonCharacterStateMachine::Tick(float DeltaTime)
{
	
	if (CurrentState == nullptr) return;
	
	CurrentState->StateTick(DeltaTime);
}

void UDragonCharacterStateMachine::FindStates()
{
	TArray<UActorComponent*> FoundComponents = Character->K2_GetComponentsByClass(UDragonCharacterState::StaticClass());

	for (UActorComponent* Component : FoundComponents)
	{
		UDragonCharacterState* State = Cast<UDragonCharacterState>(Component);
		
		if (State == nullptr) continue;
		
		if (State->GetStateID() == EDragonCharacterStateID::None) continue;
		
		AllStates.Add(State);
	}
}

void UDragonCharacterStateMachine::InitStates()
{
	for (UDragonCharacterState* State : AllStates)
	{
		State->StateInit(this);
	}
}
