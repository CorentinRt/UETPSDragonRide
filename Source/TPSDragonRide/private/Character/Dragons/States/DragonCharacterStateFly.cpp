// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragons/States/DragonCharacterStateFly.h"

#include "Character/Dragons/DragonCharacter.h"
#include "Character/Dragons/DragonCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


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

	Character->GetCharacterMovement()->MaxWalkSpeed = FlySpeed;
	
	if (FlyMontage != nullptr)
	{
		Character->PlayAnimMontage(FlyMontage);
	}
}

void UDragonCharacterStateFly::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);

	Character->GetCharacterMovement()->GravityScale = 1.0f;
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
	else if (Character->InputMoveValue.Size() > 0.1f)
	{
		FRotator TempRot = Character->GetActorRotation();

		TempRot.Pitch -= Character->InputMoveValue.Y * DeltaTime * 100.f;

		TempRot.Roll += Character->InputMoveValue.X * DeltaTime * 100.f;
		
		if (TempRot.Pitch < -85.f)
		{
			TempRot.Pitch = -85.f;
		}
		else if (TempRot.Pitch > 85.f)
		{
			TempRot.Pitch = 85.f;
		}
		
		Character->SetActorRotation(TempRot);

		AdaptGravityToFly();

		Character->AddMovementInput(Character->GetActorForwardVector() * 100.f);
	}
}

void UDragonCharacterStateFly::AdaptGravityToFly()
{
	if (Character == nullptr) return;
	
	if (StateMachine->GetCurrentStateID() != EDragonCharacterStateID::Fly)
	{
		Character->GetCharacterMovement()->GravityScale = 1.0f;
		return;
	}

	FRotator TempRot = Character->GetActorRotation();

	if (TempRot.Yaw > 30.f)
	{
	
		Character->GetCharacterMovement()->GravityScale = 2.0f;
	}
	else if (TempRot.Yaw < -30.f)
	{
		Character->GetCharacterMovement()->GravityScale = 0.03f;
	}
	else
	{
		Character->GetCharacterMovement()->GravityScale = 0.05f;
	}
}
