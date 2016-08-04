// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialProject.h"
#include "ExitKey.h"
#include "MyPlayer.h"

AExitKey::AExitKey(){
	PrimaryActorTick.bCanEverTick = false;
	bTouchInteracts = true;
	bCanInteract = true;
}

void AExitKey::OnInteract_Implementation(AActor* Caller){
	AMyPlayer* Player = Cast<AMyPlayer>(Caller);
	if(Player){
		Player->SetHasKey(true);
		Destroy();
	}
}
