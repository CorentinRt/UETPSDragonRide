// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSDragonRide/Public/Character/Dragons/DragonCharacter.h"

#include "EnhancedInputComponent.h"
#include "Character/CharacterController.h"


// Sets default values
ADragonCharacter::ADragonCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADragonCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (Controller != nullptr)
	{
		ControllerChara = Cast<ACharacterController>(Controller);
	}

	BindReceiveInputToController();
}

// Called every frame
void ADragonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADragonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
}

void ADragonCharacter::BindReceiveInputToController() const
{
	if (ControllerChara == nullptr) return;

	ControllerChara->InputMoveEvent.AddDynamic(this, &ADragonCharacter::ReceiveMoveInput);
	ControllerChara->InputLookEvent.AddDynamic(this, &ADragonCharacter::ReceiveLookInput);
	ControllerChara->InputJumpEvent.AddDynamic(this, &ADragonCharacter::ReceiveJumpInput);
}

void ADragonCharacter::ReceiveMoveInput(FVector2D MoveValue)
{
	InputMoveValue = MoveValue;
	OnDragonCharacterMoveInput.Broadcast(InputMoveValue);
}

void ADragonCharacter::ReceiveLookInput(FVector2D LookValue)
{
	InputLookValue = LookValue;
	OnDragonCharacterLookInput.Broadcast(InputLookValue);
}

void ADragonCharacter::ReceiveJumpInput(float Jumpvalue)
{
	OnDragonCharacterJumpInput.Broadcast(Jumpvalue);
}


