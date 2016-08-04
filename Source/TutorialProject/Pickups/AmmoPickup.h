// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PickupBase.h"
#include "AmmoPickup.generated.h"

UCLASS()
class TUTORIALPROJECT_API AAmmoPickup : public APickupBase{
	GENERATED_BODY()
public:
	virtual void OnInteract_Implementation(AActor* Caller) override;

private:
	UPROPERTY(EditDefaultsOnly)
	int32 AmmoAmount;
	UPROPERTY(EditDefaultsOnly)
	EAmmoType AmmoType;
};
