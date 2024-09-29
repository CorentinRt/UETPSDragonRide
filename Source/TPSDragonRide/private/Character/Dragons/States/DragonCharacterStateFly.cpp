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

	Character->GetCharacterMovement()->GravityScale = 0.f;
	
	CurrentFlySpeed = Character->GetCharacterMovement()->Velocity.Size();
	
	
	if (FlyMontage != nullptr)
	{
		Character->PlayAnimMontage(FlyMontage);
	}

	Character->OnDragonCharacterDiveInput.AddDynamic(this, &UDragonCharacterStateFly::OnReceiveInputDive);
	Character->OnDragonCharacterBoostFlyInput.AddDynamic(this, &UDragonCharacterStateFly::OnReceiveInputBoostFly);
}

void UDragonCharacterStateFly::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);

	if (Character == nullptr) return;

	if (!(NextState == EDragonCharacterStateID::Fly || NextState == EDragonCharacterStateID::Fall || NextState == EDragonCharacterStateID::Dive || NextState == EDragonCharacterStateID::BoostFly))
	{
		CurrentGravityApplied = 0.f;
	}
	
	if (!(NextState == EDragonCharacterStateID::Fly || NextState == EDragonCharacterStateID::Dive || NextState == EDragonCharacterStateID::BoostFly))
	{
		Character->GetCharacterMovement()->GravityScale = 1.0f;
	}

	Character->OnDragonCharacterDiveInput.RemoveDynamic(this, &UDragonCharacterStateFly::OnReceiveInputDive);
	Character->OnDragonCharacterBoostFlyInput.RemoveDynamic(this, &UDragonCharacterStateFly::OnReceiveInputBoostFly);
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
		/*
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

		*/
	}

	HandleFlyRotation(DeltaTime);
	HandleFly(DeltaTime);
}

void UDragonCharacterStateFly::HandleFlyRotation(float DeltaTime)
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

void UDragonCharacterStateFly::HandleFly(float DeltaTime)
{
	if (Character == nullptr) return;

	FVector TempCharaFwd = Character->GetActorForwardVector();

	FVector TempWorldFwd = Character->GetActorForwardVector();
	TempWorldFwd.Z = 0.0f;

	float DotProduct = FVector::DotProduct(TempCharaFwd, TempWorldFwd);

	float CosTheta = DotProduct / (TempCharaFwd.Size() * TempWorldFwd.Size());
	float AngleRadians = FMath::Acos(CosTheta);
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

	FVector TempDown(0.f, 0.f, -1.f);
	
	if (AngleDegrees > 50.f && TempCharaFwd.Z > 0.f)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Blue,
			TEXT("Look Up")
		);
		
		CurrentFlySpeed = FMath::FInterpTo(CurrentFlySpeed, 0.f, DeltaTime, FlySpeedAcceleration);
		CurrentGravityApplied = FMath::FInterpTo(CurrentGravityApplied, MaxGravityApplied, DeltaTime, GravityAppliedAcceleration);

		Character->GetCharacterMovement()->Velocity = (Character->GetActorForwardVector() * CurrentFlySpeed) + (TempDown * CurrentGravityApplied);
	}
	else if (AngleDegrees > 20.f && TempCharaFwd.Z < 0.f)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Green,
			TEXT("Look Down")
		);

		if (CurrentFlySpeed < MaxFlySpeed)
		{
			CurrentFlySpeed = FMath::FInterpTo(CurrentFlySpeed, MaxFlySpeed, DeltaTime, FlySpeedAcceleration);
		}
		CurrentGravityApplied = FMath::FInterpTo(CurrentGravityApplied, 0.f, DeltaTime, GravityAppliedAcceleration);

		Character->GetCharacterMovement()->Velocity = (Character->GetActorForwardVector() * CurrentFlySpeed) + (TempDown * CurrentGravityApplied);
	}
	else
	{
		if (CurrentFlySpeed < MaxFlySpeed / 2.f)
		{
			CurrentFlySpeed = FMath::FInterpTo(CurrentFlySpeed, MaxFlySpeed / 2.f, DeltaTime, FlySpeedAcceleration);
		}
		
		Character->GetCharacterMovement()->Velocity = (Character->GetActorForwardVector() * CurrentFlySpeed) + (TempDown * CurrentGravityApplied / 2.f);
	}

	GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Red,
			FString::Printf(TEXT("Fly Speed: %f"), CurrentFlySpeed)
		);
	GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Yellow,
			FString::Printf(TEXT("Gravity : %f"), CurrentGravityApplied)
		);
}

void UDragonCharacterStateFly::OnReceiveInputDive(float DiveValue)
{
	if (StateMachine == nullptr) return;

	StateMachine->ChangeState(EDragonCharacterStateID::Dive);
}

void UDragonCharacterStateFly::OnReceiveInputBoostFly(float BoostFlyValue)
{
	if (StateMachine == nullptr) return;

	StateMachine->ChangeState(EDragonCharacterStateID::BoostFly);
}
