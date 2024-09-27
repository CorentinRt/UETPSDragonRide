// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragons/States/DragonCharacterStateJump.h"

#include "Character/Dragons/DragonCharacter.h"


EDragonCharacterStateID UDragonCharacterStateJump::GetStateID() const
{
	return EDragonCharacterStateID::Jump;
}

void UDragonCharacterStateJump::StateInit(UDragonCharacterStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UDragonCharacterStateJump::StateEnter(EDragonCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Emerald,
		TEXT("EnterJump")
	);
	
	if (Character == nullptr) return;

	Character->Jump();

	if (JumpMontage != nullptr)
	{
		Character->PlayAnimMontage(JumpMontage);
	}
}

void UDragonCharacterStateJump::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);
}

void UDragonCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}
