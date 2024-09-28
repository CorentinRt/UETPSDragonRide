// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragons/States/DragonCharacterStateFly.h"

#include "Character/Dragons/DragonCharacter.h"
#include "Character/Dragons/DragonCharacterStateMachine.h"


EDragonCharacterStateID UDragonCharacterStateFly::GetStateID() const
{
	return EDragonCharacterStateID::Fly;
}

void UDragonCharacterStateFly::StateInit(UDragonCharacterStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UDragonCharacterStateFly::StateEnter(EDragonCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Red,
			TEXT("EnterFly")
		);
	
	if (Character == nullptr) return;
	
	if (FlyMontage != nullptr)
	{
		Character->PlayAnimMontage(FlyMontage);
	}
}

void UDragonCharacterStateFly::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);
}

void UDragonCharacterStateFly::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character == nullptr) return;
	
	if (Character->InputFlyValue == 0.f)
	{
		if (StateMachine != nullptr)
		{
			StateMachine->ChangeState(EDragonCharacterStateID::Idle);
		}
	}
}
