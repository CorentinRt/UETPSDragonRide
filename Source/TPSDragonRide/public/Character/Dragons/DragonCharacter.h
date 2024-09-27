// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DragonCharacter.generated.h"

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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDragonCharacterMoveInput, FVector2D, InputMove);
	FOnDragonCharacterMoveInput OnDragonCharacterMoveInput;
	
	UFUNCTION()
	void ReceiveMoveInput(FVector2D MoveValue);

	UPROPERTY()
	FVector2D InputMoveValue;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDragonCharacterLookInput, FVector2D, InputLook);
	FOnDragonCharacterLookInput OnDragonCharacterLookInput;
	
	UFUNCTION()
	void ReceiveLookInput(FVector2D LookValue);

	UPROPERTY()
	FVector2D InputLookValue;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDragonCharacterJumpInput, float, InputJump);
	FOnDragonCharacterJumpInput OnDragonCharacterJumpInput;
	
	UFUNCTION()
	void ReceiveJumpInput(float Jumpvalue);

protected:
	UPROPERTY()
	TObjectPtr<ACharacterController> ControllerChara;
};
