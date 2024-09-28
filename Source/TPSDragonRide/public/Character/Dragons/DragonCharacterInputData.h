// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DragonCharacterInputData.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class TPSDRAGONRIDE_API UDragonCharacterInputData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputMove;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputLook;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputJump;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputFly;
};
