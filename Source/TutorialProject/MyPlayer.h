// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "TutorialProject.h"
#include "MyPlayer.generated.h"

UCLASS()
class TUTORIALPROJECT_API AMyPlayer : public ACharacter {
	GENERATED_BODY()

public:
	AMyPlayer();

	/*INPUT*/
	void MoveForward(float val);
	void MoveRight(float val);
	void StartRun();
	void StopRun();
	void StartFire();
	void StopFire();
	void SwitchToAssault();
	void SwitchToLaser();
	void SwitchToRocket();

	void OnDeath();
	void Heal(float Amount);
	void AddAmmo(int32 Amount, EAmmoType AmmoType);
	void AddToInventory(class AWeaponBase* NewWeapon);
	void EquipWeapon(class AWeaponBase* WeaponToEquip);

	/*GETTERS & SETTERS*/
	void SetHasKey(bool newHasKey);
	bool GetHasKey() const;

	/*HUD*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=HUD)
	void HUDUpdateHP();

	/*SAVE*/
	void SetDataFromSave(const struct FSaveGameStruct& SaveData);
	struct FSaveGameStruct GetDataForSave();

	void SpawnInventory(const struct FSaveGameStruct& SaveData);

	/*OVERRIDES*/
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UCameraComponent* PlayerCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	USpringArmComponent* CameraBoom;

private:
	UPROPERTY(EditDefaultsOnly)
	float MaxHealthPoints = 100.f;
	UPROPERTY(EditDefaultsOnly)
	float WalkSpeed = 400.f;
	UPROPERTY(EditDefaultsOnly)
	float RunSpeed = 800.f;
	UPROPERTY(EditDefaultsOnly)
	uint32 bEquipNewWeapon : 1;
	UPROPERTY(EditDefaultsOnly)
	FName WeaponSocketName;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category=Player)
	float Bearing;
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category=Health)
	float HealthPoints;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"), Category=Hack)
	FPlayerInventory Inventory;

	uint32 bIsRunning : 1;
	uint32 bHasKey : 1;
};
