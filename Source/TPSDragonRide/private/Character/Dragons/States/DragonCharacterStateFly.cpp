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
		// Obtenir la rotation actuelle du dragon
		FRotator CurrentRotation = Character->GetActorRotation();

		// Calcul de la direction vers laquelle on veut tourner
		float TargetPitch = CurrentRotation.Pitch - Character->InputMoveValue.Y * DeltaTime * 500.f;
		TargetPitch = FMath::Clamp(TargetPitch, -85.f, 85.f);  // Limiter le Pitch entre -85 et 85 degrés

		float TargetRoll = Character->InputMoveValue.X * 120.f;  // Inclinaison latérale
		TargetRoll = FMath::Clamp(TargetRoll, -30.f, 30.f);  // Limiter le Roll à -30 et 30 degrés

		// Rotation de Yaw (autour de l'axe vertical, pour tourner)
		FRotator TargetRotation = FRotator(TargetPitch, CurrentRotation.Yaw, TargetRoll);
        
		// Interpolation fluide vers la nouvelle rotation
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 8.0f);  // Change la vitesse d'interpolation ici (3.0f pour plus de douceur)

		// Appliquer la nouvelle rotation au dragon
		Character->SetActorRotation(NewRotation);


	}
	Character->AddMovementInput(Character->GetActorForwardVector() * 1000.f);
	AdaptGravityToFly();
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

	if (TempRot.Pitch > 30.f)
	{
	
		Character->GetCharacterMovement()->GravityScale = 0.1f;
	}
	else if (TempRot.Pitch < -30.f)
	{
		Character->GetCharacterMovement()->GravityScale = 0.4f;
	}
	else
	{
		Character->GetCharacterMovement()->GravityScale = 0.02f;
	}
}
