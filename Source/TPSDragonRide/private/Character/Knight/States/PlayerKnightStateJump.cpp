// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Knight/States/PlayerKnightStateJump.h"

#include "Character/Knight/PlayerKnightCharacter.h"
#include "Character/Knight/PlayerKnightStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

EPlayerKnightStateID UPlayerKnightStateJump::GetStateID() const
{
	return EPlayerKnightStateID::JUMP;
}

void UPlayerKnightStateJump::StateInit(UPlayerKnightStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
	
}

void UPlayerKnightStateJump::StateEnter(EPlayerKnightStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Emerald,
		TEXT("Enter Jump")
	);
	
	Jump();
	
	EndJump();
}

void UPlayerKnightStateJump::StateEnter(EPlayerKnightStateID PreviousState, int SubStateID)
{
	Super::StateEnter(PreviousState, SubStateID);
	
}

void UPlayerKnightStateJump::StateExit(EPlayerKnightStateID NextState)
{
	Super::StateExit(NextState);
	
}

void UPlayerKnightStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
}

void UPlayerKnightStateJump::Jump()
{
	if (Character == nullptr)	return;

	if (Character->GetCharacterMovement() == nullptr)	return;
	
	if (!Character->GetCharacterMovement()->IsMovingOnGround())	return;

	GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Red,
			TEXT("EnterIdle")
		);
	
	Character->Jump();
}

void UPlayerKnightStateJump::EndJump()
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerKnightStateID::IDLE);
}
