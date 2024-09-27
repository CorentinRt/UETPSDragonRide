// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/CharacterSettings.h"
#include "Character/Dragons/DragonCharacterInputData.h"
#include "InputMappingContext.h"

void ACharacterController::BeginPlay()
{
	Super::BeginPlay();
}

UDragonCharacterInputData* ACharacterController::LoadInputDataFromConfig()
{
	const UCharacterSettings* CharacterSettings = GetDefault<UCharacterSettings>();
	if (CharacterSettings == nullptr) return nullptr;
	
	return CharacterSettings->DragonInputData.LoadSynchronous();
}

UInputMappingContext* ACharacterController::LoadInputMappingContextFromConfig()
{
	const UCharacterSettings* CharacterSettings = GetDefault<UCharacterSettings>();
	if (CharacterSettings == nullptr) return nullptr;

	return CharacterSettings->InputMappingContext.LoadSynchronous();
}

void ACharacterController::SetupMappingContextIntoController() const
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();

	if (LocalPlayer == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (InputSystem == nullptr) return;
	if (InputMappingContext == nullptr) return;
	
	InputSystem->AddMappingContext(InputMappingContext, 0);

}

void ACharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputData = LoadInputDataFromConfig();
	InputMappingContext = LoadInputMappingContextFromConfig();
	
	SetupMappingContextIntoController();
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (EnhancedInputComponent == nullptr) return;

	BindMoveAction(EnhancedInputComponent);
	BindLookAction(EnhancedInputComponent);
}

void ACharacterController::MoveAction(const FInputActionValue& InputActionValue)
{
	FVector2D MoveValue = InputActionValue.Get<FVector2D>();

	InputMoveEvent.Broadcast(MoveValue);
}

void ACharacterController::BindMoveAction(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (EnhancedInputComponent == nullptr) return;

	EnhancedInputComponent->BindAction(
		InputData->InputMove,
		ETriggerEvent::Started,
		this,
		&ACharacterController::MoveAction
		);
	EnhancedInputComponent->BindAction(
		InputData->InputMove,
		ETriggerEvent::Triggered,
		this,
		&ACharacterController::MoveAction
		);
	EnhancedInputComponent->BindAction(
		InputData->InputMove,
		ETriggerEvent::Completed,
		this,
		&ACharacterController::MoveAction
		);
}

void ACharacterController::LookAction(const FInputActionValue& InputActionValue)
{
	FVector2D LookValue = InputActionValue.Get<FVector2D>();

	InputLookEvent.Broadcast(LookValue);
}

void ACharacterController::BindLookAction(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (EnhancedInputComponent == nullptr) return;

	EnhancedInputComponent->BindAction(
		InputData->InputLook,
		ETriggerEvent::Started,
		this,
		&ACharacterController::LookAction
		);
}
