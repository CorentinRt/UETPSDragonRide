// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSDragonRide/Public/Character/Dragons/DragonCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/CharacterController.h"
#include "Character/CharacterSettings.h"
#include "Character/Dragons/DragonCharacterStateMachine.h"
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

	SpringArmComponent->TargetArmLength = 300.f;

	SpringArmComponent->SetRelativeLocation(FVector(0, 0, 50.f));
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

	CreateStateMachine();
	InitStateMachine();

	InitLookSensitivity();
}

// Called every frame
void ADragonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickStateMachine(DeltaTime);
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
	UpdateLookDir(LookValue, GetWorld()->GetDeltaSeconds());
	OnDragonCharacterLookInput.Broadcast(InputLookValue);
}

void ADragonCharacter::InitLookSensitivity()
{
	const UCharacterSettings* CharacterSettings = GetDefault<UCharacterSettings>();

	if (CharacterSettings == nullptr) return;
	
	LookHorizontalSensitivity = GetDefault<UCharacterSettings>()->MouseHorizontalSensitivity;
	LookHorizontalSensitivity = GetDefault<UCharacterSettings>()->MouseVerticalSensitivity;
}

void ADragonCharacter::UpdateLookDir(FVector2D LookDir, float DeltaTime)
{
	if (SpringArmComponent == nullptr) return;

	FRotator TempRot = SpringArmComponent->GetRelativeRotation();
	
	SpringArmComponent->SetRelativeRotation(TempRot + FRotator(LookDir.Y * DeltaTime * LookVerticalSensitivity, LookDir.X * DeltaTime * LookHorizontalSensitivity, 0));
}

void ADragonCharacter::ReceiveJumpInput(float Jumpvalue)
{
	OnDragonCharacterJumpInput.Broadcast(Jumpvalue);
}

void ADragonCharacter::CreateStateMachine()
{
	StateMachine = NewObject<UDragonCharacterStateMachine>();
}

void ADragonCharacter::InitStateMachine()
{
	if (StateMachine == nullptr) return;
	
	StateMachine->Init(this);
}

void ADragonCharacter::TickStateMachine(float DeltaTime) const
{
	if (StateMachine == nullptr) return;
	
	StateMachine->Tick(DeltaTime);
}


