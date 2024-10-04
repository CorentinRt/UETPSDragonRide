// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragons/States/DragonCharacterStateJump.h"

#include "Character/Dragons/DragonCharacter.h"
#include "Character/Dragons/DragonCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


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

	Character->Jump();	// Jump

	if (JumpMontage != nullptr)
	{
		Character->PlayAnimMontage(JumpMontage);	// Play Anim
	}
}

void UDragonCharacterStateJump::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);
}

void UDragonCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character == nullptr) return;
	if (StateMachine == nullptr) return;
	
	if (Character->GetVelocity().Z < 0.f)	// velocity fall -> Fall
	{
		StateMachine->ChangeState(EDragonCharacterStateID::Fall);
	}
	else if (Character->GetCharacterMovement()->IsMovingOnGround())	// On Ground -> Idle
	{
		StateMachine->ChangeState(EDragonCharacterStateID::Idle);
	}
}
