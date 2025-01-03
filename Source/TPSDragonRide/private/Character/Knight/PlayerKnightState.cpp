// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Knight/PlayerKnightState.h"

#include "Character/Knight/PlayerKnightStateMachine.h"
#include "Character/CharacterSettings.h"


EPlayerKnightStateID UPlayerKnightState::GetStateID() const
{
	return EPlayerKnightStateID::NONE;
}

void UPlayerKnightState::StateInit(UPlayerKnightStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;

	if (StateMachine != nullptr)
	{
		Character = StateMachine->GetCharacter();
	}

	CharacterSettings = GetDefault<UCharacterSettings>();
}

void UPlayerKnightState::StateEnter(EPlayerKnightStateID PreviousState)
{
	
}

void UPlayerKnightState::StateEnter(EPlayerKnightStateID PreviousState, int SubStateID)
{
	StateEnter(PreviousState);
}

void UPlayerKnightState::StateExit(EPlayerKnightStateID NextState)
{
	
}

void UPlayerKnightState::StateTick(float DeltaTime)
{
	
}
