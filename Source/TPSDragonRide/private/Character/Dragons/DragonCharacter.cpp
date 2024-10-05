// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSDragonRide/Public/Character/Dragons/DragonCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/CharacterController.h"
#include "Character/CharacterSettings.h"
#include "Character/Dragons/DragonCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ADragonCharacter::ADragonCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);
	SpringArmComponent->TargetArmLength = 2000.f;
	SpringArmComponent->SetRelativeLocation(FVector(0, 0, 700.f));
}

// Called when the game starts or when spawned
void ADragonCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (Controller != nullptr)
	{
		ControllerChara = Cast<ACharacterController>(Controller);
	}

	if (CameraComponent != nullptr)
	{
		CameraDefaultPosition = CameraComponent->GetRelativeLocation();
	}
	
	BindReceiveInputToController();

	CreateStateMachine();
	InitStateMachine();

	InitLookSensitivity();
}

// Called every frame
void ADragonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickStateMachine(DeltaTime);

	CenterLookDir(DeltaTime);
	HandleCameraPosition(DeltaTime);
}

// Called to bind functionality to input
void ADragonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
}


#pragma region ReceiveInputFromController
void ADragonCharacter::BindReceiveInputToController() const	// Bind controller's Input to Actions
{
	if (ControllerChara == nullptr) return;

	ControllerChara->InputMoveEvent.AddDynamic(this, &ADragonCharacter::ReceiveMoveInput);
	ControllerChara->InputLookEvent.AddDynamic(this, &ADragonCharacter::ReceiveLookInput);
	ControllerChara->InputJumpEvent.AddDynamic(this, &ADragonCharacter::ReceiveJumpInput);
	ControllerChara->InputFlyEvent.AddDynamic(this, &ADragonCharacter::ReceiveFlyInput);
	ControllerChara->InputDiveEvent.AddDynamic(this, &ADragonCharacter::ReceiveDiveInput);
	ControllerChara->InputBoostFlyEvent.AddDynamic(this, &ADragonCharacter::ReceiveBoostFlyInput);
}

void ADragonCharacter::ReceiveMoveInput(FVector2D MoveValue)	// Receive Move
{
	InputMoveValue = MoveValue;
	OnDragonCharacterMoveInput.Broadcast(InputMoveValue);
}


#pragma region Look
void ADragonCharacter::ReceiveLookInput(FVector2D LookValue)	// Receive Look
{
	InputLookValue = LookValue;
	UpdateLookDir(LookValue, GetWorld()->GetDeltaSeconds());
	OnDragonCharacterLookInput.Broadcast(InputLookValue);
}

void ADragonCharacter::InitLookSensitivity()	// Init sensitivity
{
	const UCharacterSettings* CharacterSettings = GetDefault<UCharacterSettings>();

	if (CharacterSettings == nullptr) return;
	
	LookHorizontalSensitivity = GetDefault<UCharacterSettings>()->MouseHorizontalSensitivity;
	LookHorizontalSensitivity = GetDefault<UCharacterSettings>()->MouseVerticalSensitivity;
}

FRotator ADragonCharacter::GetLookRotation()	// Return look rotation
{
	if (SpringArmComponent == nullptr) return FRotator();

	return SpringArmComponent->GetComponentRotation();
}


void ADragonCharacter::UpdateLookDir(FVector2D LookDir, float DeltaTime)	// Manager rotations look behaviors
{
	if (SpringArmComponent == nullptr) return;

	HasUpdatedLookDir = true;	// avoids auto center if updated
	
	FRotator TempRot = SpringArmComponent->GetRelativeRotation();
	FRotator AddRot(0.f, 0.f, 0.f);

	if (FMath::Abs(TempRot.Pitch) < 70.f)	// Pitch < 70 -> rotate springArm Up/Down
	{
		AddRot.Pitch = LookDir.Y * DeltaTime * LookVerticalSensitivity;
	}

	AddRot.Yaw = LookDir.X * DeltaTime * LookHorizontalSensitivity;	// Rotate springArm Left/Right
	
	SpringArmComponent->SetRelativeRotation(TempRot + AddRot);
}

void ADragonCharacter::CenterLookDir(float DeltaTime)	// Auto center Look
{
	if (SpringArmComponent == nullptr || HasUpdatedLookDir)	// look not updated -> center
	{
		HasUpdatedLookDir = false;
		return;
	}

	// Smooth center
	FRotator TempRot = FMath::RInterpTo(SpringArmComponent->GetRelativeRotation(), FRotator(-15.f, 0.f, 0.f), DeltaTime, 3.f);

	SpringArmComponent->SetRelativeRotation(TempRot);
}

void ADragonCharacter::LockLookDirYaw()	// Lock spring arm Yaw + Roll
{
	if (SpringArmComponent == nullptr) return;

	SpringArmComponent->SetRelativeRotation(FRotator(SpringArmComponent->GetRelativeRotation().Pitch, 0.f, 0.f));
}
#pragma endregion 

#pragma region Jump
void ADragonCharacter::ReceiveJumpInput(float Jumpvalue)	// Receive Jump
{
	OnDragonCharacterJumpInput.Broadcast(Jumpvalue);
}

#pragma endregion

#pragma region Fly
void ADragonCharacter::ReceiveFlyInput(float FlyValue)	// Receive Fly
{
	InputFlyValue = FlyValue;
	OnDragonCharacterFlyInput.Broadcast(InputFlyValue);
}

#pragma endregion

#pragma region Dive
void ADragonCharacter::ReceiveDiveInput(float DiveValue)	// Receive Dive
{
	InputDiveValue = DiveValue;
	OnDragonCharacterDiveInput.Broadcast(InputDiveValue);
}

#pragma endregion

#pragma region BoostFly
void ADragonCharacter::ReceiveBoostFlyInput(float BoostValue)	// Receive Boost Fly
{
	OnDragonCharacterBoostFlyInput.Broadcast(BoostValue);
}

#pragma endregion

#pragma endregion 


#pragma region Camera
void ADragonCharacter::HandleCameraPosition(float DeltaTime)	// Manager Camera Offsets
{
	if (CameraComponent == nullptr) return;
	
	FVector CurrentRelLocation = CameraComponent->GetRelativeLocation();

	CurrentRelLocation.Y = FMath::FInterpTo(CurrentRelLocation.Y, (CameraYOffsets + CameraDefaultPosition.Y) * CurrentCameraPosition, DeltaTime, 2.f);

	/*
	GEngine->AddOnScreenDebugMessage(
				-1,
				3.f,
				FColor::Orange,
				FString::Printf(TEXT("CameraXOffset: %f"), CurrentRelLocation.Y)
			);
	*/
	
	CameraComponent->SetRelativeLocation(CurrentRelLocation);
}

void ADragonCharacter::SetCameraTargetPositionToLeft()	// Camera to Left
{
	if (CameraComponent == nullptr) return;
	if (CurrentCameraPosition == -1) return;

	CurrentCameraPosition = -1;
}

void ADragonCharacter::SetCameraTargetPositionToCenter()	// Camera to Center
{
	if (CameraComponent == nullptr) return;
	if (CurrentCameraPosition == 0) return;

	CurrentCameraPosition = 0;
}

void ADragonCharacter::SetCameraTargetPositionToRight()	// Camera to Right
{
	if (CameraComponent == nullptr) return;
	if (CurrentCameraPosition == 1) return;

	CurrentCameraPosition = 1;
}

#pragma endregion

#pragma region State Machine
void ADragonCharacter::CreateStateMachine()
{
	StateMachine = NewObject<UDragonCharacterStateMachine>();
}

void ADragonCharacter::InitStateMachine()	// Call Init StateMachine
{
	if (StateMachine == nullptr) return;
	
	StateMachine->Init(this);
}

void ADragonCharacter::TickStateMachine(float DeltaTime) const	// Call Tick StateMachine
{
	if (StateMachine == nullptr) return;
	
	StateMachine->Tick(DeltaTime);
}
#pragma endregion


