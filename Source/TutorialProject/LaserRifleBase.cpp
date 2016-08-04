// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialProject.h"
#include "LaserRifleBase.h"

void ALaserRifleBase::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	if(!bIsFiring && !HasFullAmmo()){
		Charge += RechargeAmount*DeltaTime;
		if(Charge > 1){
			AddAmmo((int32)Charge);
			Charge -= 1;
		}
	}
}
