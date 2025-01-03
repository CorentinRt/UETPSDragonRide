// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerKnightCharacter.generated.h"

class UPlayerKnightStateMachine;
class UPlayerKnightState;
class USpringArmComponent;
class UCameraComponent;
class ACharacterController;

UCLASS()
class TPSDRAGONRIDE_API APlayerKnightCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerKnightCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void BindReceiveInputToController() const;


	UPROPERTY()
	TObjectPtr<ACharacterController> ControllerChara;

#pragma region StateMachine

private:
	void CreateStateMachine();
	void InitStateMachine();
	
public:
	UPROPERTY()
	TObjectPtr<UPlayerKnightStateMachine> StateMachine;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UPlayerKnightState>> PlayerKnightStatesClass;
#pragma endregion

	
#pragma region Components

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

#pragma endregion
	

	
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

	UPROPERTY()
	bool UseLookInFront = false;

	UFUNCTION()
	void SetLookInFront(bool Value);

	UFUNCTION()
	bool GetLookInFront();
	
	UFUNCTION()
	void InitLookSensitivity();

	UFUNCTION()
	FRotator GetLookRotation();
	
	UFUNCTION()
	void UpdateLookDir(FVector2D LookDir, float DeltaTime);
	
	UFUNCTION()
	void CenterLookDir(float DeltaTime);

	UFUNCTION()
	void UpdateLookInFrontDir(FVector2D LookInFrontDir, float DeltaTime);
	
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
	
};
