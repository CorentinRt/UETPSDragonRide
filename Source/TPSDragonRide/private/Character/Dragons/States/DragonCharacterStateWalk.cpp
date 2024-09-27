// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragons/States/DragonCharacterStateWalk.h"

#include "Character/Dragons/DragonCharacter.h"
#include "Character/Dragons/DragonCharacterStateMachine.h"
#include "GameFramework/PawnMovementComponent.h"


EDragonCharacterStateID UDragonCharacterStateWalk::GetStateID() const
{
	return EDragonCharacterStateID::Walk;
}

void UDragonCharacterStateWalk::StateInit(UDragonCharacterStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UDragonCharacterStateWalk::StateEnter(EDragonCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Emerald,
		TEXT("EnterWalk")
	);
	
	if (Character == nullptr) return;
	
	if (WalkMontage != nullptr)
	{
		Character->PlayAnimMontage(WalkMontage);
	}

	Character->OnDragonCharacterJumpInput.AddDynamic(this, &UDragonCharacterStateWalk::OnReceiveInputJump);
}

void UDragonCharacterStateWalk::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);

	if (Character == nullptr) return;
	
	Character->OnDragonCharacterJumpInput.RemoveDynamic(this, &UDragonCharacterStateWalk::OnReceiveInputJump);
}

void UDragonCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character == nullptr) return;
	if (StateMachine == nullptr) return;

	if (Character->InputMoveValue.Size() <= 0.1f)
	{
		StateMachine->ChangeState(EDragonCharacterStateID::Idle);
	}
	else
	{
		FVector Dir(Character->InputMoveValue.X, Character->InputMoveValue.Y, 0.f);
		Character->GetMovementComponent()->AddInputVector(Dir);
	}
}

void UDragonCharacterStateWalk::OnReceiveInputJump(float InputJump)
{
	if (StateMachine == nullptr) return;

	StateMachine->ChangeState(EDragonCharacterStateID::Jump);
}
