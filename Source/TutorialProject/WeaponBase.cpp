// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialProject.h"
#include "WeaponBase.h"
#include "MyPlayer.h"

AWeaponBase::AWeaponBase(){
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachTo(RootComponent);

	TraceParams = FCollisionQueryParams(FName(TEXT("HitscanTrace")), true, this);
	//TraceParams.TraceTag = FName("HitscanTrace");
}

void AWeaponBase::BeginPlay(){
	Super::BeginPlay();

	//GetWorld()->DebugDrawTraceTag = FName("HitscanTrace");

	if(bSpawnFull) CurrentAmmo = MaxAmmo;
}

void AWeaponBase::AddAmmo(float Amount){
	CurrentAmmo = FMath::Clamp(CurrentAmmo+Amount, CurrentAmmo, MaxAmmo);
}

void AWeaponBase::SetAmmo(float Amount){
	CurrentAmmo = Amount;
}

void AWeaponBase::DealDamage(const FHitResult& Hit){
	if(Hit.GetActor()){
		float DealtDamage = BaseDamage;
		FVector ShotDir = GetActorLocation() - Hit.ImpactPoint;

		FPointDamageEvent DamageEvent;
		DamageEvent.Damage = DealtDamage;
		DamageEvent.HitInfo = Hit;
		DamageEvent.ShotDirection = ShotDir;
		DamageEvent.ShotDirection.Normalize();

		Hit.GetActor()->TakeDamage(DealtDamage, DamageEvent, OwningPlayer->GetController(), this);
	}
}

void AWeaponBase::StartFire(){
	if(CurrentAmmo > 0){
		//UE_LOG(LogTemp, Warning, TEXT("StartedFire!"));
		bIsFiring = true;
		DoFire();
		float TimerDelay = FireRate > 0 ? 1/(FireRate*0.01667) : FApp::GetDeltaTime();

		if(!FireRateHandle.IsValid())
			GetWorld()->GetTimerManager().SetTimer(FireRateHandle, this, &AWeaponBase::StartFire, TimerDelay, true);
	}
	else {
		StopFire();
	}
}

void AWeaponBase::StopFire(){
	//UE_LOG(LogTemp, Warning, TEXT("StoppedFire!"));
	bIsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(FireRateHandle);
	FireRateHandle.Invalidate();
}

void AWeaponBase::DoFire(){
	FHitResult Hit(ForceInit);
	FVector Start = WeaponMesh->GetSocketLocation(MuzzleSocketName);
	FVector End = Start + CalcSpread() * MaxRange;

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Weapon, TraceParams);

	UE_LOG(LogTemp, Warning, TEXT("Fire!"));

	CurrentAmmo--;
	SpawnFireEffect();

	if(Hit.GetActor()){
		UE_LOG(LogTemp, Warning, TEXT("Hit! %s"), *Hit.GetActor()->GetName());
		DealDamage(Hit);
		SpawnImpactEffect(Hit);
	}
}

FVector AWeaponBase::CalcSpread(){
	if(OwningPlayer) {
		FVector Direction = OwningPlayer->GetActorRotation().Vector();
		float Angle = atan( Spread/10000 );
		return FMath::VRandCone(Direction, Angle);
	}
	else {
		FVector Direction = GetActorRotation().Vector();
		float Angle = atan( Spread/10000 );
		return FMath::VRandCone(Direction, Angle);
	}
}

void AWeaponBase::SpawnFireEffect(){
	FVector Location = WeaponMesh->GetSocketLocation(MuzzleSocketName);
	FRotator Rotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
	UGameplayStatics::SpawnEmitterAttached(ShotEffect, WeaponMesh, MuzzleSocketName, Location, Rotation, EAttachLocation::KeepWorldPosition, true);
	UGameplayStatics::PlaySoundAttached(ShotSound, WeaponMesh, MuzzleSocketName, Location, EAttachLocation::KeepWorldPosition, true, 1, 1, 0);
}

void AWeaponBase::SpawnImpactEffect(const FHitResult& Hit){
	FVector Location = Hit.ImpactPoint;
	FRotator Rotation = Hit.ImpactNormal.Rotation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Location, Rotation, true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location, Rotation, 1, 1, 0);
}

void AWeaponBase::ChangeOwner(AActor* NewOwner){
	AMyPlayer* Player = Cast<AMyPlayer>(NewOwner);
	if(Player){
		OwningPlayer = Player;
	}
	SetOwner(NewOwner);
}

void AWeaponBase::OnInteract_Implementation(AActor* Caller){
	AMyPlayer* Player = Cast<AMyPlayer>(Caller);
	if(Player){
		Player->AddToInventory(this);
	}
}

void AWeaponBase::GetAmmo(float& Current, float& Max) const{
	Current = CurrentAmmo;
	Max = MaxAmmo;
}
bool AWeaponBase::HasFullAmmo() const{
	return CurrentAmmo == MaxAmmo ? true : false;
}

float AWeaponBase::GetCurrentAmmo() const{
	return CurrentAmmo;
}
