﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragons/States/DragonCharacterStateFall.h"

#include "Character/Dragons/DragonCharacter.h"
#include "Character/Dragons/DragonCharacterStateMachine.h"
#include "GameFramework/PawnMovementComponent.h"


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

	GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Emerald,
			TEXT("EnterFall")
		);
	
	if (Character == nullptr) return;

	if (FallMontage != nullptr)
	{
		Character->PlayAnimMontage(FallMontage);	// Play Anim
	}

	Character->OnDragonCharacterFlyInput.AddDynamic(this, &UDragonCharacterStateFall::OnReceiveInputFly);	// Bind fly
}

void UDragonCharacterStateFall::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);

	Character->OnDragonCharacterFlyInput.RemoveDynamic(this, &UDragonCharacterStateFall::OnReceiveInputFly);
}

void UDragonCharacterStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character == nullptr) return;
	
	if (Character->GetMovementComponent()->IsMovingOnGround())	// On Ground -> Idle
	{
		if (StateMachine == nullptr) return;
		
		StateMachine->ChangeState(EDragonCharacterStateID::Idle);
	}
}

void UDragonCharacterStateFall::OnReceiveInputFly(float InputFly)	// press fly -> Fly
{
	if (StateMachine == nullptr) return;
	
	StateMachine->ChangeState(EDragonCharacterStateID::Fly);
}
