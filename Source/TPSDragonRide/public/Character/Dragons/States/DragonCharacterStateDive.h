// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Dragons/DragonCharacterState.h"
#include "DragonCharacterStateDive.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPSDRAGONRIDE_API UDragonCharacterStateDive : public UDragonCharacterState
{
	GENERATED_BODY()

public:

	virtual EDragonCharacterStateID GetStateID() const override;
	
	virtual void StateInit(UDragonCharacterStateMachine* InStateMachine) override;

	virtual void StateEnter(EDragonCharacterStateID PreviousState) override;

	virtual void StateExit(EDragonCharacterStateID NextState) override;

	virtual void StateTick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UAnimMontage* DiveMontage;
};
