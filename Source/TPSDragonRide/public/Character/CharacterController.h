﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "CharacterController.generated.h"

class UDragonCharacterInputData;
class UInputMappingContext;
class UCharacterSettings;
/**
 * 
 */
UCLASS()
class TPSDRAGONRIDE_API ACharacterController : public APlayerController
{
	GENERATED_BODY()

#pragma region Mapping Context / Input Data
	
public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<UDragonCharacterInputData> InputData;

protected:
	virtual void BeginPlay() override;

	UDragonCharacterInputData* LoadInputDataFromConfig();
	UInputMappingContext* LoadInputMappingContextFromConfig();
	
	void SetupMappingContextIntoController() const;

	virtual void SetupInputComponent() override;
#pragma endregion

#pragma region Bind Actions

protected:
	void MoveAction(const FInputActionValue& InputActionValue);
	void BindMoveAction(UEnhancedInputComponent* EnhancedInputComponent);


	void LookAction(const FInputActionValue& InputActionValue);
	void BindLookAction(UEnhancedInputComponent* EnhancedInputComponent);

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveEvent, FVector2D, InputMove);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputLookEvent, FVector2D, InputMove);

	FInputMoveEvent InputMoveEvent;
	FInputLookEvent InputLookEvent;

#pragma endregion
};
