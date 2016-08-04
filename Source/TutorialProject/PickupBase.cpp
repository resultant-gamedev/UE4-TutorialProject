// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialProject.h"
#include "PickupBase.h"


APickupBase::APickupBase(){
	PrimaryActorTick.bCanEverTick = true;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;
	CollisionSphere->SetCollisionProfileName("OverlapAllDynamic");
	PickupMesh->AttachTo(RootComponent);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APickupBase::BeginPlay(){
	Super::BeginPlay();

	if(bTouchInteracts){
		UE_LOG(LogTemp, Warning, TEXT("Adding Delegate"));
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnBeginOverlap);
	}

}

void APickupBase::Tick(float DeltaTime){
	Super::Tick( DeltaTime );

}

void APickupBase::OnInteract_Implementation(AActor* Caller){

}

void APickupBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(bCanInteract){
		OnInteract(OtherActor);
	}
}

void APickupBase::SetCanInteract(bool NewInteract){
	bCanInteract = NewInteract;
}
