// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TutorialProject.h"
#include "PickupBase.generated.h"

UCLASS()
class TUTORIALPROJECT_API APickupBase : public AActor{
	GENERATED_BODY()

public:
	APickupBase();

	UFUNCTION(BlueprintNativeEvent, Category=Interaction)
	void OnInteract(AActor* Caller);
	virtual void OnInteract_Implementation(AActor* Caller);

	void SetCanInteract(bool NewInteract);

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* PickupMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	USphereComponent* CollisionSphere;
	UPROPERTY(EditDefaultsOnly)
	uint32 bCanInteract : 1;
	UPROPERTY(EditDefaultsOnly)
	uint32 bTouchInteracts : 1;
};
