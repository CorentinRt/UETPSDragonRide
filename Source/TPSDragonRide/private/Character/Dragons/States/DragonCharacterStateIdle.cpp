// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragons/States/DragonCharacterStateIdle.h"

#include "Character/Dragons/DragonCharacter.h"
#include "Character/Dragons/DragonCharacterStateMachine.h"


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

	if (IdleMontage != nullptr)
	{
		Character->PlayAnimMontage(IdleMontage);
	}

	
}

void UDragonCharacterStateIdle::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);
}

void UDragonCharacterStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

void UDragonCharacterStateIdle::OnReceiveInputJump(float InputJump)
{
	if (StateMachine == nullptr) return;
	
	StateMachine->ChangeState(EDragonCharacterStateID::Jump);
}


