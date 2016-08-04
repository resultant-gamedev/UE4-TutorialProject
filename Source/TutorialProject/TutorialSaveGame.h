// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "TutorialSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FWeaponSaveStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SaveGame)
	TSubclassOf<class AWeaponBase> Class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SaveGame)
	int32 Ammo;
};

USTRUCT(BlueprintType)
struct FSaveGameStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SaveGame)
	FString Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SaveGame)
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SaveGame)
	int CurrentWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SaveGame)
	FWeaponSaveStruct AssaultRifle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SaveGame)
	FWeaponSaveStruct LaserRifle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SaveGame)
	FWeaponSaveStruct RocketLauncher;
};

UCLASS()
class TUTORIALPROJECT_API UTutorialSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UTutorialSaveGame();

	UPROPERTY(VisibleAnywhere, Category="SaveGame")
	FString SlotName;
	UPROPERTY(VisibleAnywhere, Category="SaveGame")
	int32 UserIndex;
	UPROPERTY(VisibleAnywhere, Category="SaveGame")
	FSaveGameStruct SaveInfo;

	static UTutorialSaveGame* CreateSaveInstance();
	void Save();
	UTutorialSaveGame* Load(FString Slot, int32 Index);
};
