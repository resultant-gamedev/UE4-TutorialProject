// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialProject.h"
#include "MyHealthPickup.h"
#include "MyPlayer.h"

void AMyHealthPickup::OnInteract_Implementation(AActor* Caller){
	AMyPlayer* Player = Cast<AMyPlayer>(Caller);
	if(Player){
		Player->Heal(HealAmount);
		Destroy();
	}
}
