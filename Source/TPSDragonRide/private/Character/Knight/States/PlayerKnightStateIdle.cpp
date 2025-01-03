// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Knight/States/PlayerKnightStateIdle.h"

#include "Character/Knight/PlayerKnightCharacter.h"
#include "Character/Knight/PlayerKnightStateMachine.h"

EPlayerKnightStateID UPlayerKnightStateIdle::GetStateID() const
{
	return EPlayerKnightStateID::IDLE;
}

void UPlayerKnightStateIdle::StateInit(UPlayerKnightStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
	
}

void UPlayerKnightStateIdle::StateEnter(EPlayerKnightStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Emerald,
		TEXT("EnterIdle")
	);
	
	if (Character == nullptr) return;
	
	if (IdleMontage != nullptr)
	{
		Character->PlayAnimMontage(IdleMontage);	// Play Anim
	}
}

void UPlayerKnightStateIdle::StateEnter(EPlayerKnightStateID PreviousState, int SubStateID)
{
	Super::StateEnter(PreviousState, SubStateID);
	
}

void UPlayerKnightStateIdle::StateExit(EPlayerKnightStateID NextState)
{
	Super::StateExit(NextState);
	
}

void UPlayerKnightStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	CheckStartWalking();
}

void UPlayerKnightStateIdle::CheckStartWalking()
{
	if (Character == nullptr) return;
	
	if (Character->InputMoveValue.Size() > 0.1f)	// Joystick moving -> Walk
	{
		if (StateMachine == nullptr) return;
		
		StateMachine->ChangeState(EPlayerKnightStateID::WALKING);
	}
	else if (Character->GetVelocity().Z < 0.f)	// velocity fall -> Fall
	{
		if (StateMachine == nullptr) return;
		
		StateMachine->ChangeState(EPlayerKnightStateID::FALLING);
	}

	GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Emerald,
			FString::Printf(TEXT("Walking: %f / %f"), Character->InputMoveValue.X, Character->InputMoveValue.Y)
		);
}
