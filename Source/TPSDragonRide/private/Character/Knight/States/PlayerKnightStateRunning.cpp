// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Knight/States/PlayerKnightStateRunning.h"

EPlayerKnightStateID UPlayerKnightStateRunning::GetStateID() const
{
	return EPlayerKnightStateID::RUNNING;
}

void UPlayerKnightStateRunning::StateInit(UPlayerKnightStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
	
}

void UPlayerKnightStateRunning::StateEnter(EPlayerKnightStateID PreviousState)
{
	Super::StateEnter(PreviousState);
	
}

void UPlayerKnightStateRunning::StateEnter(EPlayerKnightStateID PreviousState, int SubStateID)
{
	Super::StateEnter(PreviousState, SubStateID);
	
}

void UPlayerKnightStateRunning::StateExit(EPlayerKnightStateID NextState)
{
	Super::StateExit(NextState);
	
}

void UPlayerKnightStateRunning::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
}
