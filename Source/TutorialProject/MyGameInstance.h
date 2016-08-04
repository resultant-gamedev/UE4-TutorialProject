// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

UCLASS()
class TUTORIALPROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	FString LastSaveLoaded;
	bool bShouldLoadSave = false;	
};
