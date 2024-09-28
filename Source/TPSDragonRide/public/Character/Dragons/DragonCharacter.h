// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DragonCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDragonCharacterStateMachine;
class ACharacterController;
class UDragonCharacterInputData;
class UInputMappingContext;

UCLASS()
class TPSDRAGONRIDE_API ADragonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADragonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void BindReceiveInputToController() const;


#pragma region Move
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDragonCharacterMoveInput, FVector2D, InputMove);
	FOnDragonCharacterMoveInput OnDragonCharacterMoveInput;
	
	UFUNCTION()
	void ReceiveMoveInput(FVector2D MoveValue);

	UPROPERTY()
	FVector2D InputMoveValue;

#pragma endregion

#pragma region Look
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDragonCharacterLookInput, FVector2D, InputLook);
	FOnDragonCharacterLookInput OnDragonCharacterLookInput;
	
	UFUNCTION()
	void ReceiveLookInput(FVector2D LookValue);

	UPROPERTY()
	FVector2D InputLookValue;

	UPROPERTY()
	float LookVerticalSensitivity = 100.f;

	UPROPERTY()
	float LookHorizontalSensitivity = 100.f;

	UFUNCTION()
	void InitLookSensitivity();

	UFUNCTION()
	FRotator GetLookRotation();
	
	UFUNCTION()
	void UpdateLookDir(FVector2D LookDir, float DeltaTime);

	UFUNCTION()
	void CenterLookDir(float DeltaTime);

	UFUNCTION()
	void LockLookDirYaw();

	bool HasUpdatedLookDir = false;
	
#pragma endregion

#pragma region Jump
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDragonCharacterJumpInput, float, InputJump);
	FOnDragonCharacterJumpInput OnDragonCharacterJumpInput;
	
	UFUNCTION()
	void ReceiveJumpInput(float Jumpvalue);

#pragma endregion

#pragma region Fly

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDragonCharacterFlyInput, float, InputFly);
	FOnDragonCharacterFlyInput OnDragonCharacterFlyInput;
	
	UFUNCTION()
	void ReceiveFlyInput(float FlyValue);

	UPROPERTY()
	float InputFlyValue;
	
#pragma endregion 

#pragma region Dive
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDragonCharacterDiveInput, float, InputDive);
	FOnDragonCharacterDiveInput OnDragonCharacterDiveInput;

	UFUNCTION()
	void ReceiveDiveInput(float DiveValue);

	UPROPERTY()
	float InputDiveValue;

#pragma endregion

	
protected:
	UPROPERTY()
	TObjectPtr<ACharacterController> ControllerChara;

	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY()
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
#pragma region State Machine
public:
	void CreateStateMachine();

	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const;

protected:
	TObjectPtr<UDragonCharacterStateMachine> StateMachine;
	
#pragma endregion
};
