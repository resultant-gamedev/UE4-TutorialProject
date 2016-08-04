// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PickupBase.h"
#include "ExitKey.generated.h"

UCLASS()
class TUTORIALPROJECT_API AExitKey : public APickupBase
{
	GENERATED_BODY()

public:
	AExitKey();

	virtual void OnInteract_Implementation(AActor* Caller) override;
};
