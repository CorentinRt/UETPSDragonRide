// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Knight/States/PlayerKnightStateFalling.h"

#include "Character/Knight/PlayerKnightCharacter.h"
#include "Character/Knight/PlayerKnightStateMachine.h"

EPlayerKnightStateID UPlayerKnightStateFalling::GetStateID() const
{
	return EPlayerKnightStateID::FALLING;
}

void UPlayerKnightStateFalling::StateInit(UPlayerKnightStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
	
}

void UPlayerKnightStateFalling::StateEnter(EPlayerKnightStateID PreviousState)
{
	Super::StateEnter(PreviousState);
	
}

void UPlayerKnightStateFalling::StateEnter(EPlayerKnightStateID PreviousState, int SubStateID)
{
	Super::StateEnter(PreviousState, SubStateID);
	
}

void UPlayerKnightStateFalling::StateExit(EPlayerKnightStateID NextState)
{
	Super::StateExit(NextState);
	
}

void UPlayerKnightStateFalling::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	CheckStillFalling();
}

void UPlayerKnightStateFalling::CheckStillFalling()
{
	if (Character->GetVelocity().Z >= 0.f)	// velocity High or null -> Idle
	{
		StateMachine->ChangeState(EPlayerKnightStateID::IDLE);
	}
}
