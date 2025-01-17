﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragons/States/DragonCharacterStateIdle.h"

#include "Character/Dragons/DragonCharacter.h"
#include "Character/Dragons/DragonCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


EDragonCharacterStateID UDragonCharacterStateIdle::GetStateID() const
{
	return EDragonCharacterStateID::Idle;
}

void UDragonCharacterStateIdle::StateInit(UDragonCharacterStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UDragonCharacterStateIdle::StateEnter(EDragonCharacterStateID PreviousState)
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

	Character->OnDragonCharacterJumpInput.AddDynamic(this, &UDragonCharacterStateIdle::OnReceiveInputJump);	// Bind Jump
}

void UDragonCharacterStateIdle::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);

	if (Character == nullptr) return;
	
	Character->OnDragonCharacterJumpInput.RemoveDynamic(this, &UDragonCharacterStateIdle::OnReceiveInputJump);
}

void UDragonCharacterStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character == nullptr) return;

	if (Character->InputMoveValue.Size() > 0.1f)	// Joystick move -> Walk
	{
		if (StateMachine == nullptr) return;
		
		StateMachine->ChangeState(EDragonCharacterStateID::Walk);
	}
	else if (Character->GetVelocity().Z < 0.f)	// velocity fall -> Fall
	{
		if (StateMachine == nullptr) return;
		
		StateMachine->ChangeState(EDragonCharacterStateID::Fall);
	}
}

void UDragonCharacterStateIdle::OnReceiveInputJump(float InputJump)	// Press Jump -> Jump
{
	if (StateMachine == nullptr) return;
	
	StateMachine->ChangeState(EDragonCharacterStateID::Jump);
}


