// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragons/States/DragonCharacterStateWalk.h"

#include "Character/Dragons/DragonCharacter.h"
#include "Character/Dragons/DragonCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"


EDragonCharacterStateID UDragonCharacterStateWalk::GetStateID() const
{
	return EDragonCharacterStateID::Walk;
}

void UDragonCharacterStateWalk::StateInit(UDragonCharacterStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UDragonCharacterStateWalk::StateEnter(EDragonCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Emerald,
		TEXT("EnterWalk")
	);
	
	if (Character == nullptr) return;

	Character->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;	// Speed = walkSpeed
	
	if (WalkMontage != nullptr)
	{
		Character->PlayAnimMontage(WalkMontage);	// Play Anim
	}

	Character->OnDragonCharacterJumpInput.AddDynamic(this, &UDragonCharacterStateWalk::OnReceiveInputJump); // Bind Jump
}

void UDragonCharacterStateWalk::StateExit(EDragonCharacterStateID NextState)
{
	Super::StateExit(NextState);

	if (Character == nullptr) return;
	
	Character->OnDragonCharacterJumpInput.RemoveDynamic(this, &UDragonCharacterStateWalk::OnReceiveInputJump);
}

void UDragonCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character == nullptr) return;
	if (StateMachine == nullptr) return;

	if (Character->InputMoveValue.Size() <= 0.1f)	// joystick not move -> idle
	{
		StateMachine->ChangeState(EDragonCharacterStateID::Idle);
	}
	else if (Character->GetVelocity().Z < 0.f)	// velocity fall -> fall
	{
		StateMachine->ChangeState(EDragonCharacterStateID::Fall);
	}
	
	HandleWalkRotation(DeltaTime);
	HandleWalk(DeltaTime);
}

void UDragonCharacterStateWalk::HandleWalk(float DeltaTime)	// Manage movements Walk behaviors
{
	FVector DirX(Character->InputMoveValue.X * Character->GetActorRightVector());
	FVector DirY(Character->InputMoveValue.Y * Character->GetActorForwardVector());
	FVector Dir = DirX + DirY;
	Character->AddMovementInput(Dir);
}

void UDragonCharacterStateWalk::HandleWalkRotation(float DeltaTime)	// Manage rotation Walk behaviors
{
	Character->SetActorRotation(FRotator(0.f, Character->GetLookRotation().Yaw, 0.f));
	Character->LockLookDirYaw();
}

void UDragonCharacterStateWalk::OnReceiveInputJump(float InputJump)	// Press Jump -> Jump
{
	if (StateMachine == nullptr) return;

	StateMachine->ChangeState(EDragonCharacterStateID::Jump);
}
