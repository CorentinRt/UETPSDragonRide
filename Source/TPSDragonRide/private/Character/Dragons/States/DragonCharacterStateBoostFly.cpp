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

	Character->SetCameraFOVToFlyBoost();
	
	if (BoostFlyMontage != nullptr)
	{
		Character->PlayAnimMontage(BoostFlyMontage);
	}

}

void UDragonCharacterStateBoostFly::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);

	CurrentBoostFlyDuration = 0.f;
	
	if (Character == nullptr) return;

	Character->SetCameraFOVToDefault();
}

void UDragonCharacterStateBoostFly::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character == nullptr) return;

	HandleBoostFlyVelocity(DeltaTime);
	HandleBoostFlyRotation(DeltaTime);
	
	HandleBoostFlyDuration(DeltaTime);
}

void UDragonCharacterStateBoostFly::HandleBoostFlyDuration(float DeltaTime)
{
	if (CurrentBoostFlyDuration >= BoostFlyDuration)
	{
		if (StateMachine == nullptr) return;
		
		if (Character->InputFlyValue != 0.f)
		{
			StateMachine->ChangeState(EDragonCharacterStateID::Fly);
		}
		else if (Character->InputDiveValue != 0.f)
		{
			StateMachine->ChangeState(EDragonCharacterStateID::Dive);
		}
		else
		{
			StateMachine->ChangeState(EDragonCharacterStateID::Fall);
		}
	}
	else
	{
		CurrentBoostFlyDuration += DeltaTime;
	}
}

void UDragonCharacterStateBoostFly::HandleBoostFlyVelocity(float DeltaTime)
{
	float CurrentVelocity = Character->GetCharacterMovement()->Velocity.Size();

	CurrentVelocity = FMath::FInterpTo(CurrentVelocity, MaxBoostFlySpeed, DeltaTime, BoostFlyAcceleration);

	Character->GetCharacterMovement()->Velocity = Character->GetActorForwardVector() * CurrentVelocity;
}

void UDragonCharacterStateBoostFly::HandleBoostFlyRotation(float DeltaTime)
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
}
