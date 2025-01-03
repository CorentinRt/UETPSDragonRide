// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Knight/States/PlayerKnightStateWalking.h"

#include "Character/Knight/PlayerKnightCharacter.h"
#include "Character/Knight/PlayerKnightStateMachine.h"

EPlayerKnightStateID UPlayerKnightStateWalking::GetStateID() const
{
	return EPlayerKnightStateID::WALKING;
}

void UPlayerKnightStateWalking::StateInit(UPlayerKnightStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
	
}

void UPlayerKnightStateWalking::StateEnter(EPlayerKnightStateID PreviousState)
{
	Super::StateEnter(PreviousState);
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Emerald,
		TEXT("EnterWalking")
	);
}

void UPlayerKnightStateWalking::StateEnter(EPlayerKnightStateID PreviousState, int SubStateID)
{
	Super::StateEnter(PreviousState, SubStateID);
	
}

void UPlayerKnightStateWalking::StateExit(EPlayerKnightStateID NextState)
{
	Super::StateExit(NextState);
	
}

void UPlayerKnightStateWalking::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	CheckStillWalking();
	
	HandleWalk(DeltaTime);
	HandleWalkRotation(DeltaTime);
}



void UPlayerKnightStateWalking::HandleWalk(float DeltaTime)	// Manage movements Walk behaviors
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Tick")
	);
	
	FVector DirX(Character->InputMoveValue.X * Character->GetActorRightVector());
	FVector DirY(Character->InputMoveValue.Y * Character->GetActorForwardVector());
	FVector Dir = DirX + DirY;
	Character->AddMovementInput(Dir);
}

void UPlayerKnightStateWalking::HandleWalkRotation(float DeltaTime)	// Manage rotation Walk behaviors
{
	Character->SetActorRotation(FRotator(0.f, Character->GetLookRotation().Yaw, 0.f));
	Character->LockLookDirYaw();
}

void UPlayerKnightStateWalking::CheckStillWalking()
{
	if (Character->InputMoveValue.Size() <= 0.1f)	// joystick not moving -> idle
	{
		StateMachine->ChangeState(EPlayerKnightStateID::IDLE);
	}
	else if (Character->GetVelocity().Z < 0.f)	// velocity fall -> fall
	{
		StateMachine->ChangeState(EPlayerKnightStateID::FALLING);
	}
}
