// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialProject.h"
#include "ExitDoor.h"
#include "MyPlayer.h"
#include "TutorialSaveGame.h"
#include "MyGameInstance.h"

AExitDoor::AExitDoor(){
	PrimaryActorTick.bCanEverTick = true;
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	OpenTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("OpenTrigger"));
	LoadTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("LoadTrigger"));

	RootComponent = DoorMesh;
	OpenTrigger->AttachTo(RootComponent);
	LoadTrigger->AttachTo(RootComponent);
}

void AExitDoor::BeginPlay(){
	Super::BeginPlay();
	SetActorTickEnabled(false);
	OpenTrigger->OnComponentBeginOverlap.AddDynamic(this, &AExitDoor::OnBeginOverlap);
	LoadTrigger->OnComponentBeginOverlap.AddDynamic(this, &AExitDoor::OnBeginOverlap);
}

void AExitDoor::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);

	if(bShouldLerp){
		SetActorLocation( FMath::VInterpTo(GetActorLocation(), NewLoc, DeltaSeconds, LerpSpeed) );
	}
}

void AExitDoor::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyPlayer* Player = Cast<AMyPlayer>(OtherActor);

	if(OverlappedComp == OpenTrigger && Player){
		if(Player->GetHasKey()){
			Player->SetHasKey(false);
			NewLoc = GetActorLocation() + FVector(0,0,250);
			bShouldLerp = true;
			SetActorTickEnabled(true);
			UE_LOG(LogTemp, Warning, TEXT("ShouldOpen"));
		}
	}
	else if(OverlappedComp==LoadTrigger && Player){
		UTutorialSaveGame* Instance = UTutorialSaveGame::CreateSaveInstance();
		Instance->SaveInfo = Player->GetDataForSave();
		LevelToLoad.ToString( Instance->SaveInfo.Level );
		Instance->Save();

		UMyGameInstance* MyGI = Cast<UMyGameInstance>(GetGameInstance());
		if(MyGI){
			MyGI->bShouldLoadSave = true;
			MyGI->LastSaveLoaded = "Autosave";
		}

		UGameplayStatics::OpenLevel(GetWorld(), LevelToLoad, false, "");
	}
}
