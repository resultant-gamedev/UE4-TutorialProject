// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PickupBase.h"
#include "MyHealthPickup.generated.h"


UCLASS()
class TUTORIALPROJECT_API AMyHealthPickup : public APickupBase{
	GENERATED_BODY()
public:
	virtual void OnInteract_Implementation(AActor* Caller) override;

private:
	UPROPERTY(EditDefaultsOnly)
	float HealAmount = 25.f;

};
