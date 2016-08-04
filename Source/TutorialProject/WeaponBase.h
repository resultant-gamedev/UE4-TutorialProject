// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PickupBase.h"
#include "WeaponBase.generated.h"

UCLASS()
class TUTORIALPROJECT_API AWeaponBase : public APickupBase{
	GENERATED_BODY()
public:
	AWeaponBase();

	void AddAmmo(float Amount);
	void SetAmmo(float Amount);
	void DealDamage(const FHitResult& Hit);
	void StartFire();
	void StopFire();
	virtual void DoFire();
	FVector CalcSpread();
	void SpawnFireEffect();
	void SpawnImpactEffect(const FHitResult& Hit);
	UFUNCTION(BlueprintCallable, Category=Weapon)
	void ChangeOwner(AActor* NewOwner);

	UFUNCTION(BlueprintCallable, Category=Weapon)
	void GetAmmo(float& Current, float& Max) const;
	bool HasFullAmmo() const;

	float GetCurrentAmmo() const;

	virtual void OnInteract_Implementation(AActor* Caller) override;
	virtual void BeginPlay() override;

	bool bSpawnFull = true;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="bUsesProjectile"))
	TSubclassOf<class AProjectileBase> ProjectileClass;
	UPROPERTY(EditDefaultsOnly)
	FName MuzzleSocketName;

	uint32 bIsFiring : 1;
	float CurrentAmmo = 0;
	class AMyPlayer* OwningPlayer;

private:
	UPROPERTY(EditDefaultsOnly)
	EAmmoType AmmoType;
	UPROPERTY(EditDefaultsOnly)
	float MaxAmmo = 0;
	UPROPERTY(EditDefaultsOnly)
	float Spread;
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="!bUsesProjectile"))
	float BaseDamage;
	UPROPERTY(EditDefaultsOnly)
	float FireRate;
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="!bUsesProjectile"))
	float MaxRange;
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ShotEffect;
	UPROPERTY(EditDefaultsOnly)
	USoundCue* ShotSound;
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="!bUsesProjectile"))
	UParticleSystem* ImpactEffect;
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="!bUsesProjectile"))
	USoundCue* ImpactSound;
	UPROPERTY(EditDefaultsOnly)
	uint32 bUsesProjectile : 1;

	FCollisionQueryParams TraceParams;
	FTimerHandle FireRateHandle;
};
