// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragons/States/DragonCharacterStateDive.h"

#include "Character/Dragons/DragonCharacter.h"
#include "Character/Dragons/DragonCharacterStateMachine.h"


EDragonCharacterStateID UDragonCharacterStateDive::GetStateID() const
{
	return EDragonCharacterStateID::Dive;
}

void UDragonCharacterStateDive::StateInit(UDragonCharacterStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UDragonCharacterStateDive::StateEnter(EDragonCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Emerald,
			TEXT("EnterDive")
		);
	
	if (Character == nullptr) return;

	if (DiveMontage != nullptr)
	{
		Character->PlayAnimMontage(DiveMontage);
	}

	
}

void UDragonCharacterStateDive::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);
}

void UDragonCharacterStateDive::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character == nullptr) return;
	if (StateMachine == nullptr) return;
	
	if (Character->InputDiveValue == 0.f)
	{
		if (Character->InputFlyValue != 0.f)
		{
			StateMachine->ChangeState(EDragonCharacterStateID::Fly);
		}
		else
		{
			StateMachine->ChangeState(EDragonCharacterStateID::Fall);
		}
	}
}
