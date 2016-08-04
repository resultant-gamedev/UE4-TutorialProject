// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponBase.h"
#include "LaserRifleBase.generated.h"

UCLASS()
class TUTORIALPROJECT_API ALaserRifleBase : public AWeaponBase{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly) //per second
	float RechargeAmount;
	float Charge;
};
