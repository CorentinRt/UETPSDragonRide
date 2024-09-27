// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CharacterSettings.generated.h"

class UInputMappingContext;
class UDragonCharacterInputData;
/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Character Settings"))
class TPSDRAGONRIDE_API UCharacterSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Inputs")
	TSoftObjectPtr<UDragonCharacterInputData> DragonInputData;

	UPROPERTY(Config, EditAnywhere, Category="Inputs")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(Config, EditAnywhere, Category="Sensitivity")
	float MouseVerticalSensitivity;

	UPROPERTY(Config, EditAnywhere, Category="Sensitivity")
	float MouseHorizontalSensitivity;
};
