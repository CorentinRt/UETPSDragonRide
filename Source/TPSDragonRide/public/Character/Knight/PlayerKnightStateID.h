// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerKnightStateID.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerKnightStateID : uint8
{
	NONE = 0,
	IDLE,
	WALKING,
	RUNNING,
	CROUCHING,
	FALLING,
	JUMP
};
