// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialProject.h"
#include "ProjectileBase.h"


AProjectileBase::AProjectileBase(){
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileParticles"));
	RootComponent = ProjectileMesh;
	ProjectileParticles->AttachTo(RootComponent);

	TraceParams = FCollisionQueryParams(FName(TEXT("ProjectileTrace")), false, this);
}

void AProjectileBase::BeginPlay(){
	Super::BeginPlay();

}

void AProjectileBase::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	Trace(DeltaTime);
	Move(DeltaTime);
}


void AProjectileBase::Inititalize(FVector Direction){
	Velocity = Direction * InitialSpeed;
}

void AProjectileBase::Trace(float DeltaTime){
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FVector End = Start + Velocity*DeltaTime;

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	if(Hit.GetActor()){
		SpawnImpactEffect(Hit);
		DealDamage(Hit);
		Destroy();
	}
}

void AProjectileBase::Move(float DeltaTime){
	SetActorLocation(GetActorLocation() + Velocity*DeltaTime);
}

void AProjectileBase::DealDamage(FHitResult& Hit){
	TArray<FHitResult> Hits;

	FVector Start = Hit.ImpactPoint;
	FVector End = Start + FVector(0,0,1);
	GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Weapon, FCollisionShape::MakeSphere(RadDamageParams.OuterRadius), TraceParams);

	FRadialDamageEvent RadDmgEvent;
	RadDmgEvent.Params = RadDamageParams;
	RadDmgEvent.Origin = Hit.ImpactPoint;
	RadDmgEvent.ComponentHits = Hits;

	for(FHitResult nHit : Hits){
		nHit.GetActor()->TakeDamage(RadDamageParams.BaseDamage, RadDmgEvent, Instigator->GetController(), this);
		if(nHit.GetComponent()){
			FVector Direction = nHit.GetActor()->GetActorLocation() - RadDmgEvent.Origin;
			Direction.Normalize();
			nHit.GetComponent()->AddImpulse(Direction*ImpulseStrength, "", true);
		}
	}
}

void AProjectileBase::SpawnImpactEffect(FHitResult& Hit){
	FVector Location = Hit.ImpactPoint;
	FRotator Rotation = Hit.ImpactNormal.Rotation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Location, Rotation, true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location, Rotation, 1, 1, 0);
}
