// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragons/States/DragonCharacterStateDive.h"

#include "Character/Dragons/DragonCharacter.h"
#include "Character/Dragons/DragonCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


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

	CurrentDiveSpeed = Character->GetCharacterMovement()->Velocity.Size();
	
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
	else
	{
		// FRotator CurrentRotation = Character->GetActorRotation();
		//
		// // Rotation Plongée
		// float TargetPitch = -90.f;
		//
		// // Rotation Tonneau
		// float TargetRoll = CurrentRotation.Roll + Character->InputMoveValue.X * DeltaTime * 1500.f;
		//
		// // Smooth Rotation
		// FRotator TargetRotation = FRotator(TargetPitch, CurrentRotation.Yaw, TargetRoll);
		// FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 8.0f);
		// Character->SetActorRotation(NewRotation);

		// GEngine->AddOnScreenDebugMessage(
		// 	-1,
		// 	3.f,
		// 	FColor::Emerald,
		// 	FString::Printf(TEXT("Dive: %f"), TargetPitch)
		// );
		
		// FVector NewVelocity = Character->GetCharacterMovement()->Velocity;
		// NewVelocity *= 0.8f;
		//
		// FVector ForwardDirection = Character->GetActorForwardVector();
		//
		// NewVelocity += ForwardDirection * 3500.f;
		//
		// Character->GetCharacterMovement()->Velocity = NewVelocity;
	}

	HandleDiveRotation(DeltaTime);
	HandleDive(DeltaTime);
}

void UDragonCharacterStateDive::HandleDiveRotation(float DeltaTime)
{
	FRotator CurrentRotation = Character->GetActorRotation();

	// Rotation Plongée
	float TargetPitch = -90.f;

	// Rotation Tonneau
	float TargetRoll = CurrentRotation.Roll + Character->InputMoveValue.X * DeltaTime * 1500.f;

	// Smooth Rotation
	FRotator TargetRotation = FRotator(TargetPitch, CurrentRotation.Yaw, TargetRoll);
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 8.0f);
	Character->SetActorRotation(NewRotation);
}

void UDragonCharacterStateDive::HandleDive(float DeltaTime)
{
	if (Character == nullptr) return;
	
	CurrentDiveSpeed = FMath::FInterpTo(CurrentDiveSpeed, MaxDiveSpeed, DeltaTime, DiveSpeedAcceleration);

	Character->GetCharacterMovement()->Velocity = Character->GetActorForwardVector() * CurrentDiveSpeed;

	GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Purple,
			FString::Printf(TEXT("Dive: %f"), CurrentDiveSpeed)
		);
}
