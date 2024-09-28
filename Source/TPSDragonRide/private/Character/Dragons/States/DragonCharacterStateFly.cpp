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

	Character->OnDragonCharacterDiveInput.AddDynamic(this, &UDragonCharacterStateFly::OnReceiveInputDive);
}

void UDragonCharacterStateFly::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);

	if (Character == nullptr) return;
	
	Character->GetCharacterMovement()->GravityScale = 1.0f;

	Character->OnDragonCharacterDiveInput.RemoveDynamic(this, &UDragonCharacterStateFly::OnReceiveInputDive);
}

void UDragonCharacterStateFly::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character == nullptr) return;
	
	if (Character->InputFlyValue == 0.f)
	{
		if (StateMachine != nullptr)
		{
			if (Character->GetCharacterMovement()->IsMovingOnGround())
			{
				StateMachine->ChangeState(EDragonCharacterStateID::Idle);
			}
			else
			{
				StateMachine->ChangeState(EDragonCharacterStateID::Fall);
			}
		}
	}
	else
	{
		FRotator CurrentRotation = Character->GetActorRotation();

		// Rotation Plongée
		float TargetPitch = CurrentRotation.Pitch - Character->InputMoveValue.Y * DeltaTime * 1000.f;
		TargetPitch = FMath::Clamp(TargetPitch, -60.f, 60.f);

		// Rotation Tonneau
		float TargetRoll = Character->InputMoveValue.X * 60.f;
		TargetRoll = FMath::Clamp(TargetRoll, -60.f, 60.f);

		// Rotation Tourner
		float TargetYaw = CurrentRotation.Yaw + Character->InputMoveValue.X * DeltaTime * 1500.f;

		// Smooth Rotation
		FRotator TargetRotation = FRotator(TargetPitch, TargetYaw, TargetRoll);
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 8.0f);
		Character->SetActorRotation(NewRotation);

		
		FVector NewVelocity = Character->GetCharacterMovement()->Velocity;
		NewVelocity *= 0.8f;

		FVector ForwardDirection = Character->GetActorForwardVector();
		
		NewVelocity += ForwardDirection * 2500.f;

		Character->GetCharacterMovement()->Velocity = NewVelocity;
	}
}

void UDragonCharacterStateFly::OnReceiveInputDive(float DiveValue)
{
	if (StateMachine == nullptr) return;

	StateMachine->ChangeState(EDragonCharacterStateID::Dive);
}
