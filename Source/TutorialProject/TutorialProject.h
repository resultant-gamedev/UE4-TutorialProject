// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define ECC_Weapon ECC_GameTraceChannel1

#include "Engine.h"
#include "TutorialProject.generated.h"

UENUM(BlueprintType)
enum class EStencilColor : uint8
{
	SC_Green = 250 UMETA(DisplayName="Green"),
	SC_Blue = 251 UMETA(DisplayName="Blue"),
	SC_Red = 252 UMETA(DisplayName="Red"),
	SC_White = 253 UMETA(DisplayName="White")
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	AT_Bullets UMETA(DisplayName="Bullets"),
	AT_Rockets UMETA(DisplayName="Rockets"),
	AT_Lasers UMETA(DisplayName="Lasers")
};

USTRUCT(BlueprintType)
struct FPlayerInventory
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Player)
	class AWeaponBase* CurrentWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Player)
	class AAssaultRifleBase* AssaultRifle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Player)
	class ALaserRifleBase* LaserRifle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Player)
	class ARocketLauncherBase* RocketLauncher;
};
