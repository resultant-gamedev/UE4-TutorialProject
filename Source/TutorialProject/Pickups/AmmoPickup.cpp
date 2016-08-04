// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialProject.h"
#include "AmmoPickup.h"
#include "MyPlayer.h"

void AAmmoPickup::OnInteract_Implementation(AActor* Caller){
	AMyPlayer* Player = Cast<AMyPlayer>(Caller);
	if(Player){
		Player->AddAmmo(AmmoAmount, AmmoType);
	}
	Destroy();
}
