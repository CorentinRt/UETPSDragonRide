// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Knight/PlayerKnightCharacter.h"

#include "Camera/CameraComponent.h"
#include "Character/CharacterController.h"
#include "Character/CharacterSettings.h"
#include "Character/Knight/PlayerKnightStateMachine.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
APlayerKnightCharacter::APlayerKnightCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));

	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void APlayerKnightCharacter::BeginPlay()
{
	Super::BeginPlay();

	CreateStateMachine();
	InitStateMachine();

	
}

// Called every frame
void APlayerKnightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (StateMachine != nullptr)
	{
		StateMachine->Tick(DeltaTime);
	}
}

// Called to bind functionality to input
void APlayerKnightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (Controller != nullptr)
	{
		ControllerChara = Cast<ACharacterController>(Controller);
	}
	
	BindReceiveInputToController();
}

void APlayerKnightCharacter::BindReceiveInputToController() const	// Bind controller's Input to Actions
{
	if (ControllerChara == nullptr) return;
	
	ControllerChara->InputMoveEvent.AddDynamic(this, &APlayerKnightCharacter::ReceiveMoveInput);
	ControllerChara->InputLookEvent.AddDynamic(this, &APlayerKnightCharacter::ReceiveLookInput);
	ControllerChara->InputJumpEvent.AddDynamic(this, &APlayerKnightCharacter::ReceiveJumpInput);
}

void APlayerKnightCharacter::CreateStateMachine()
{
	StateMachine = NewObject<UPlayerKnightStateMachine>(this);
}

void APlayerKnightCharacter::InitStateMachine()
{
	if (StateMachine == nullptr) return;

	StateMachine->Init(this);
}

void APlayerKnightCharacter::ReceiveMoveInput(FVector2D MoveValue)	// Receive Move
{
	InputMoveValue = MoveValue;
	OnDragonCharacterMoveInput.Broadcast(InputMoveValue);
}


#pragma region Look
void APlayerKnightCharacter::ReceiveLookInput(FVector2D LookValue)	// Receive Look
{
	InputLookValue = LookValue;
	UpdateLookDir(LookValue, GetWorld()->GetDeltaSeconds());
	OnDragonCharacterLookInput.Broadcast(InputLookValue);
}

void APlayerKnightCharacter::SetLookInFront(bool Value)
{
	UseLookInFront = Value;
}

bool APlayerKnightCharacter::GetLookInFront()
{
	return UseLookInFront;
}

void APlayerKnightCharacter::InitLookSensitivity()	// Init sensitivity
{
	const UCharacterSettings* CharacterSettings = GetDefault<UCharacterSettings>();

	if (CharacterSettings == nullptr) return;
	
	LookHorizontalSensitivity = GetDefault<UCharacterSettings>()->MouseHorizontalSensitivity;
	LookHorizontalSensitivity = GetDefault<UCharacterSettings>()->MouseVerticalSensitivity;
}

FRotator APlayerKnightCharacter::GetLookRotation()	// Return look rotation
{
	if (SpringArmComponent == nullptr) return FRotator();

	return SpringArmComponent->GetComponentRotation();
}


void APlayerKnightCharacter::UpdateLookDir(FVector2D LookDir, float DeltaTime)	// Manager rotations look behaviors
{
	if (SpringArmComponent == nullptr) return;

	HasUpdatedLookDir = true;	// avoids auto center if updated

	if (UseLookInFront)
	{
		UpdateLookInFrontDir(LookDir, DeltaTime);
		return;
	}
	
	
	FRotator TempRot = SpringArmComponent->GetRelativeRotation();
	FRotator AddRot(0.f, 0.f, 0.f);

	if (FMath::Abs(TempRot.Pitch + (LookDir.Y * DeltaTime * LookVerticalSensitivity)) < 70.f)	// Pitch < 70 -> rotate springArm Up/Down
	{
		AddRot.Pitch = LookDir.Y * DeltaTime * LookVerticalSensitivity;
	}

	AddRot.Yaw = LookDir.X * DeltaTime * LookHorizontalSensitivity;	// Rotate springArm Left/Right
	
	SpringArmComponent->SetRelativeRotation(TempRot + AddRot);
}


void APlayerKnightCharacter::CenterLookDir(float DeltaTime)	// Auto center Look
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

void APlayerKnightCharacter::UpdateLookInFrontDir(FVector2D LookInFrontDir, float DeltaTime)
{
	
}

void APlayerKnightCharacter::LockLookDirYaw()	// Lock spring arm Yaw + Roll
{
	if (SpringArmComponent == nullptr) return;

	SpringArmComponent->SetRelativeRotation(FRotator(SpringArmComponent->GetRelativeRotation().Pitch, 0.f, 0.f));
}
#pragma endregion 

#pragma region Jump
void APlayerKnightCharacter::ReceiveJumpInput(float Jumpvalue)	// Receive Jump
{
	OnDragonCharacterJumpInput.Broadcast(Jumpvalue);
}

#pragma endregion


