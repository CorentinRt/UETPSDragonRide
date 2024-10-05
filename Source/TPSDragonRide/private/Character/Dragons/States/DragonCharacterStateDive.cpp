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

	
	CurrentDiveSpeed = Character->GetCharacterMovement()->Velocity.Size();	// dive speed = velocity -> smooth transition
	Character->SetCameraFOVToFlyBoost();	// FOV -> fly Boost

	
	if (DiveMontage != nullptr)
	{
		Character->PlayAnimMontage(DiveMontage);	// Play anim
	}
	
}

void UDragonCharacterStateDive::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);

	if (Character == nullptr) return;

	Character->SetCameraFOVToDefault();	// Reset FOV
	
}

void UDragonCharacterStateDive::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character == nullptr) return;
	if (StateMachine == nullptr) return;
	
	if (Character->InputDiveValue == 0.f)	// stop pressing dive
	{
		if (Character->InputFlyValue != 0.f)	// press fly -> Fly
		{
			StateMachine->ChangeState(EDragonCharacterStateID::Fly);
		}
		else    // -> Fall
		{
			StateMachine->ChangeState(EDragonCharacterStateID::Fall);
		}
	}

	HandleDiveRotation(DeltaTime);
	HandleDive(DeltaTime);
}

void UDragonCharacterStateDive::HandleDiveRotation(float DeltaTime)	// Manager rotation Dive behaviors
{
	FRotator CurrentRotation = Character->GetActorRotation();

	float TargetPitch = -80.f;	// Rotation Dive

	//float TargetRoll = CurrentRotation.Roll + Character->InputMoveValue.X * DeltaTime * 1500.f;	// Rotation Roll (not used)

	float TargetYaw = CurrentRotation.Yaw + Character->InputMoveValue.X * DeltaTime * 1500.f; // Rotation Yaw (used because of rotator's execution order)
	
	
	//FRotator TargetRotation = FRotator(TargetPitch, CurrentRotation.Yaw, TargetRoll); // (not used -> see target yaw)
	FRotator TargetRotation = FRotator(TargetPitch, TargetYaw, 0.f);	// test fix rot
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 8.0f);	// Smooth Rotation
	Character->SetActorRotation(NewRotation);
}

void UDragonCharacterStateDive::HandleDive(float DeltaTime)	// Manager movements Dive behaviors
{
	if (Character == nullptr) return;
	
	CurrentDiveSpeed = FMath::FInterpTo(CurrentDiveSpeed, MaxDiveSpeed, DeltaTime, DiveSpeedAcceleration);

	Character->GetCharacterMovement()->Velocity = Character->GetActorForwardVector() * CurrentDiveSpeed;

	/*
	GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Purple,
			FString::Printf(TEXT("Dive: %f"), CurrentDiveSpeed)
		);
	*/
}
