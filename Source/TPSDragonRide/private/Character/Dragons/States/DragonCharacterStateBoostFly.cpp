// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragons/States/DragonCharacterStateBoostFly.h"

#include "Camera/CameraComponent.h"
#include "Character/Dragons/DragonCharacter.h"
#include "Character/Dragons/DragonCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


EDragonCharacterStateID UDragonCharacterStateBoostFly::GetStateID() const
{
	return EDragonCharacterStateID::BoostFly;
}

void UDragonCharacterStateBoostFly::StateInit(UDragonCharacterStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UDragonCharacterStateBoostFly::StateEnter(EDragonCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Black,
			TEXT("Enter Boost Fly")
		);
	
	if (Character == nullptr) return;

	Character->SetCameraFOVToFlyBoost();	// FOV FlyBoost
	
	if (BoostFlyMontage != nullptr)
	{
		Character->PlayAnimMontage(BoostFlyMontage);	// Play Anim
	}

}

void UDragonCharacterStateBoostFly::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);

	CurrentBoostFlyDuration = 0.f;	// Reset current Duration Boost Fly
	
	if (Character == nullptr) return;

	Character->SetCameraFOVToDefault();	// reset FOV
}

void UDragonCharacterStateBoostFly::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character == nullptr) return;

	HandleBoostFlyVelocity(DeltaTime);
	HandleBoostFlyRotation(DeltaTime);
	
	HandleBoostFlyDuration(DeltaTime);
}

void UDragonCharacterStateBoostFly::HandleBoostFlyDuration(float DeltaTime)	// Manage duration BoostFly
{
	if (CurrentBoostFlyDuration >= BoostFlyDuration)	// finished Duration
	{
		if (StateMachine == nullptr) return;
		
		if (Character->InputFlyValue != 0.f)	// press fly -> fly
		{
			StateMachine->ChangeState(EDragonCharacterStateID::Fly);
		}
		else if (Character->InputDiveValue != 0.f)	// PressDive -> Dive
		{
			StateMachine->ChangeState(EDragonCharacterStateID::Dive);
		}
		else   // default -> fall
		{
			StateMachine->ChangeState(EDragonCharacterStateID::Fall);
		}
	}
	else     // unfinished duration
	{
		CurrentBoostFlyDuration += DeltaTime;	// Decrement current Duration
	}
}

void UDragonCharacterStateBoostFly::HandleBoostFlyVelocity(float DeltaTime)	// Manage Smooth Velocity
{
	float CurrentVelocity = Character->GetCharacterMovement()->Velocity.Size();

	CurrentVelocity = FMath::FInterpTo(CurrentVelocity, MaxBoostFlySpeed, DeltaTime, BoostFlyAcceleration);

	Character->GetCharacterMovement()->Velocity = Character->GetActorForwardVector() * CurrentVelocity;
}

void UDragonCharacterStateBoostFly::HandleBoostFlyRotation(float DeltaTime)	// Manage rotation BoostFly behaviors
{
	FRotator CurrentRotation = Character->GetActorRotation();

	float TargetPitch = CurrentRotation.Pitch - Character->InputMoveValue.Y * DeltaTime * 1000.f;	// Rotation Dive
	TargetPitch = FMath::Clamp(TargetPitch, -60.f, 60.f);
	
	float TargetRoll = Character->InputMoveValue.X * 60.f;	// Rotation Roll
	TargetRoll = FMath::Clamp(TargetRoll, -60.f, 60.f);
	
	float TargetYaw = CurrentRotation.Yaw + Character->InputMoveValue.X * DeltaTime * 1500.f;	// Rotation Turn
	
	FRotator TargetRotation = FRotator(TargetPitch, TargetYaw, TargetRoll);
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 8.0f);	// Smooth Rotation
	Character->SetActorRotation(NewRotation);
}
