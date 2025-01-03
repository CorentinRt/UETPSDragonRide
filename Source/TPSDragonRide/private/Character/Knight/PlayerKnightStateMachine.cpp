// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Knight/PlayerKnightStateMachine.h"

#include "Character/Knight/PlayerKnightCharacter.h"
#include "Character/Knight/PlayerKnightState.h"

void UPlayerKnightStateMachine::Init(APlayerKnightCharacter* InCharacter)
{
	Character = InCharacter;
	
	CreateObjectStates();

	InitStates();

	ChangeState(EPlayerKnightStateID::IDLE);
}

APlayerKnightCharacter* UPlayerKnightStateMachine::GetCharacter() const
{
	return Character;
}

void UPlayerKnightStateMachine::ChangeState(EPlayerKnightStateID NextStateID, int InSubstateID)
{
	UPlayerKnightState* NextState = GetState(NextStateID);

	if (NextState == nullptr)	return;

	if (CurrentState != nullptr)
	{
		CurrentState->StateExit(NextStateID);
	}

	EPlayerKnightStateID PreviousStateID = CurrentStateID;
	CurrentStateID = NextStateID;
	CurrentState = NextState;

	if (CurrentState != nullptr)
	{
		CurrentState->StateEnter(PreviousStateID, InSubstateID);
	}
}

UPlayerKnightState* UPlayerKnightStateMachine::GetState(EPlayerKnightStateID StateID)
{
	for (UPlayerKnightState* State : AllStates)
	{
		if (State == nullptr)	continue;

		if (State->GetStateID() == StateID)	return State;
	}

	return nullptr;
}

void UPlayerKnightStateMachine::Tick(float DeltaTime)
{
	if (CurrentState == nullptr)	return;

	CurrentState->StateTick(DeltaTime);
}

UPlayerKnightState* UPlayerKnightStateMachine::GetCurrentState() const
{
	return CurrentState;
}

EPlayerKnightStateID UPlayerKnightStateMachine::GetCurrentStateID() const
{
	return CurrentStateID;
}

void UPlayerKnightStateMachine::CreateObjectStates()
{
	if (Character == nullptr)	return;
	
	TObjectPtr<UPlayerKnightState> OutState = nullptr;

	TArray<TSubclassOf<UPlayerKnightState>> OutPlayerKnightStatesClass = Character->PlayerKnightStatesClass;
	
	for (TSubclassOf<UPlayerKnightState> StateClass : OutPlayerKnightStatesClass)
	{
		OutState = NewObject<UPlayerKnightState>(this, StateClass);

		if (OutState == nullptr)	continue;

		AllStates.Add(OutState);
	}
}

void UPlayerKnightStateMachine::InitStates()
{
	for (UPlayerKnightState* State : AllStates)
	{
		if (State == nullptr)	continue;

		State->StateInit(this);
	}
}
