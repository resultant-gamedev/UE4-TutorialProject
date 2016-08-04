// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponBase.h"
#include "RocketLauncherBase.generated.h"

UCLASS()
class TUTORIALPROJECT_API ARocketLauncherBase : public AWeaponBase{
	GENERATED_BODY()

	virtual void DoFire() override;

private:
	UPROPERTY(EditDefaultsOnly)
	float ImpulseStrength;

};
