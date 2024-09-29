// Fill out your copyright notice in the Description page of Project Settings.

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


	void JumpAction(const FInputActionValue& InputActionValue);
	void BindJumpAction(UEnhancedInputComponent* EnhancedInputComponent);

	void FlyAction(const FInputActionValue& InputActionValue);
	void BindFlyAction(UEnhancedInputComponent* EnhancedInputComponent);

	void DiveAction(const FInputActionValue& InputActionValue);
	void BindDiveAction(UEnhancedInputComponent* EnhancedInputComponent);

	void BoostFlyAction(const FInputActionValue& InputActionValue);
	void BindBoostFlyAction(UEnhancedInputComponent* EnhancedInputComponent);

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveEvent, FVector2D, InputMove);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputLookEvent, FVector2D, InputLook);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputJumpEvent, float, InputJump);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputFlyEvent, float, InputFly);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputDiveEvent, float, InputDive);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputBoostFlyEvent, float, InputBoostFly);

	FInputMoveEvent InputMoveEvent;
	FInputLookEvent InputLookEvent;
	FInputJumpEvent InputJumpEvent;
	FInputJumpEvent InputFlyEvent;
	FInputJumpEvent InputDiveEvent;
	FInputBoostFlyEvent InputBoostFlyEvent;
	

#pragma endregion
};
