// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialProject.h"
#include "TutorialSaveGame.h"

UTutorialSaveGame::UTutorialSaveGame(){
	SlotName = "Autosave";
	UserIndex = 0;
}

UTutorialSaveGame* UTutorialSaveGame::CreateSaveInstance(){
	return Cast<UTutorialSaveGame>( UGameplayStatics::CreateSaveGameObject(StaticClass()) );
}

void UTutorialSaveGame::Save(){
	UGameplayStatics::SaveGameToSlot(this, SlotName, UserIndex);
}

UTutorialSaveGame* UTutorialSaveGame::Load(FString Slot, int32 Index){
	return Cast<UTutorialSaveGame>( UGameplayStatics::LoadGameFromSlot(Slot, Index) );
}
