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

	// Set velocity and gravity to smooth transitions
	Character->GetCharacterMovement()->GravityScale = 0.f;
	CurrentFlySpeed = Character->GetCharacterMovement()->Velocity.Size();
	
	Character->SetLookInFront(true);

	
	if (FlyMontage != nullptr)
	{
		Character->PlayAnimMontage(FlyMontage);	// Play Anim
	}

	Character->OnDragonCharacterDiveInput.AddDynamic(this, &UDragonCharacterStateFly::OnReceiveInputDive);	// Bind Dive
	Character->OnDragonCharacterBoostFlyInput.AddDynamic(this, &UDragonCharacterStateFly::OnReceiveInputBoostFly);	// Bind BoostFly
}

void UDragonCharacterStateFly::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);

	if (Character == nullptr) return;

	Character->SetCameraTargetPositionToCenterY();
	Character->SetCameraTargetPositionToCenterZ();
	
	
	// fly / Fall / Dive / BoostFly -> reset currentGravityApplied
	if (!(NextState == EDragonCharacterStateID::Fly || NextState == EDragonCharacterStateID::Fall || NextState == EDragonCharacterStateID::Dive || NextState == EDragonCharacterStateID::BoostFly))
	{
		CurrentGravityApplied = 0.f;
	}

	// fly / Dive / BoostFly -> gravityScale = 1
	if (!(NextState == EDragonCharacterStateID::Fly || NextState == EDragonCharacterStateID::Dive || NextState == EDragonCharacterStateID::BoostFly))
	{
		Character->GetCharacterMovement()->GravityScale = 1.0f;
		Character->SetLookInFront(false);
	}

	Character->OnDragonCharacterDiveInput.RemoveDynamic(this, &UDragonCharacterStateFly::OnReceiveInputDive);
	Character->OnDragonCharacterBoostFlyInput.RemoveDynamic(this, &UDragonCharacterStateFly::OnReceiveInputBoostFly);
}

void UDragonCharacterStateFly::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character == nullptr) return;
	
	if (Character->InputFlyValue == 0.f)	// not pressing fly
	{
		if (StateMachine != nullptr)
		{
			if (Character->GetCharacterMovement()->IsMovingOnGround())	// on Ground -> idle
			{
				StateMachine->ChangeState(EDragonCharacterStateID::Idle);
			}
			else     // not On Ground -> idle
			{
				StateMachine->ChangeState(EDragonCharacterStateID::Fall);
			}
		}
	}

	HandleFlyRotation(DeltaTime);
	HandleFly(DeltaTime);
}

void UDragonCharacterStateFly::HandleFlyRotation(float DeltaTime)	// Manage rotation fly behaviors
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

void UDragonCharacterStateFly::HandleFly(float DeltaTime)	// Manage moving fly behaviors
{
	if (Character == nullptr) return;

	
	if (Character->InputLookValue.X < -0.1f)	// if joystick left -> camera to left
	{
		Character->SetCameraTargetPositionToOffsetY(Character->InputLookValue.X);
	}
	else if (Character->InputLookValue.X > 0.1f)	// if joystick right -> camera to right
	{
		Character->SetCameraTargetPositionToOffsetY(Character->InputLookValue.X);
	}
	else     // if joystick center -> camera to center
	{
		Character->SetCameraTargetPositionToCenterY();
	}

	if (Character->InputLookValue.Y < -0.1f)	// if joystick down -> camera to bottom
	{
		Character->SetCameraTargetPositionToOffsetZ(Character->InputLookValue.Y);
	}
	else if (Character->InputLookValue.Y > 0.1f)	// if joystick up -> camera to top
	{
		Character->SetCameraTargetPositionToOffsetZ(Character->InputLookValue.Y);
	}
	else     // if joystick center -> camera to center
	{
		Character->SetCameraTargetPositionToCenterZ();
	}

	
	FVector TempCharaFwd = Character->GetActorForwardVector();

	FVector TempWorldFwd = Character->GetActorForwardVector();
	TempWorldFwd.Z = 0.0f;

	float DotProduct = FVector::DotProduct(TempCharaFwd, TempWorldFwd);	// get angle dragon by dot product

	float CosTheta = DotProduct / (TempCharaFwd.Size() * TempWorldFwd.Size());
	float AngleRadians = FMath::Acos(CosTheta);
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

	FVector TempDown(0.f, 0.f, -1.f);
	
	if (AngleDegrees > 50.f && TempCharaFwd.Z > 0.f)	// Look Up -> decrease fly speed + increase gravity applied
	{
		CurrentFlySpeed = FMath::FInterpTo(CurrentFlySpeed, 0.f, DeltaTime, FlySpeedAcceleration);
		CurrentGravityApplied = FMath::FInterpTo(CurrentGravityApplied, MaxGravityApplied, DeltaTime, GravityAppliedAcceleration);

		Character->GetCharacterMovement()->Velocity = (Character->GetActorForwardVector() * CurrentFlySpeed) + (TempDown * CurrentGravityApplied);
	}
	else if (AngleDegrees > 20.f && TempCharaFwd.Z < 0.f)	// Look Down -> increase fly speed + decrease gravity applied
	{
		if (CurrentFlySpeed < MaxFlySpeed)
		{
			CurrentFlySpeed = FMath::FInterpTo(CurrentFlySpeed, MaxFlySpeed, DeltaTime, FlySpeedAcceleration);
		}
		CurrentGravityApplied = FMath::FInterpTo(CurrentGravityApplied, 0.f, DeltaTime, GravityAppliedAcceleration);

		Character->GetCharacterMovement()->Velocity = (Character->GetActorForwardVector() * CurrentFlySpeed) + (TempDown * CurrentGravityApplied);
	}
	else    // Look default -> increase fly speed / 2
	{
		if (CurrentFlySpeed < MaxFlySpeed / 2.f)
		{
			CurrentFlySpeed = FMath::FInterpTo(CurrentFlySpeed, MaxFlySpeed / 2.f, DeltaTime, FlySpeedAcceleration);
		}
		
		Character->GetCharacterMovement()->Velocity = (Character->GetActorForwardVector() * CurrentFlySpeed) + (TempDown * CurrentGravityApplied / 2.f);
	}
	
	/*
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
	*/
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
