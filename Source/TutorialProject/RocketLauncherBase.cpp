// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialProject.h"
#include "RocketLauncherBase.h"
#include "ProjectileBase.h"
#include "MyPlayer.h"

void ARocketLauncherBase::DoFire(){
	FVector Location = WeaponMesh->GetSocketLocation(MuzzleSocketName);
	FRotator Rotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, Location, Rotation);

	CurrentAmmo--;
	SpawnFireEffect();
	OwningPlayer->GetCharacterMovement()->AddImpulse(-Rotation.Vector()*ImpulseStrength , true);

	if(Projectile){
		Projectile->SetOwner(this);
		Projectile->Instigator = OwningPlayer;
		Projectile->Inititalize(Rotation.Vector());
	}

}
