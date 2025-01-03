// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Knight/States/PlayerKnightStateRunning.h"

#include "Character/Knight/PlayerKnightCharacter.h"
#include "Character/Knight/PlayerKnightStateMachine.h"

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

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Emerald,
		TEXT("Enter Running")
	);

	if (Character != nullptr)
	{
		Character->OnPlayerKnightCharacterJumpInput.AddDynamic(this, &UPlayerKnightStateRunning::OnJump);
	}
}

void UPlayerKnightStateRunning::StateEnter(EPlayerKnightStateID PreviousState, int SubStateID)
{
	Super::StateEnter(PreviousState, SubStateID);
	
}

void UPlayerKnightStateRunning::StateExit(EPlayerKnightStateID NextState)
{
	Super::StateExit(NextState);

	
	if (Character != nullptr)
	{
		Character->OnPlayerKnightCharacterJumpInput.RemoveDynamic(this, &UPlayerKnightStateRunning::OnJump);
	}
}

void UPlayerKnightStateRunning::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
}

void UPlayerKnightStateRunning::OnJump(float InJumpValue)
{
	if (StateMachine == nullptr) return;

	StateMachine->ChangeState(EPlayerKnightStateID::JUMP);
}
