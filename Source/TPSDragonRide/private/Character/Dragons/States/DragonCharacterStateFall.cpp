// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragons/States/DragonCharacterStateFall.h"

#include "Character/Dragons/DragonCharacter.h"


EDragonCharacterStateID UDragonCharacterStateFall::GetStateID() const
{
	return EDragonCharacterStateID::Fall;
}

void UDragonCharacterStateFall::StateInit(UDragonCharacterStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UDragonCharacterStateFall::StateEnter(EDragonCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	if (Character == nullptr) return;

	if (FallMontage != nullptr)
	{
		Character->PlayAnimMontage(FallMontage);
	}
}

void UDragonCharacterStateFall::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);
}

void UDragonCharacterStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}
