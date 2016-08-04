// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialProject.h"
#include "MyPlayer.h"
#include "LaserRifleBase.h"
#include "AssaultRifleBase.h"
#include "RocketLauncherBase.h"
#include "MyGameInstance.h"
#include "TutorialSaveGame.h"

AMyPlayer::AMyPlayer(){
	PrimaryActorTick.bCanEverTick = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	PlayerCamera->AttachTo(CameraBoom);
}

void AMyPlayer::SetupPlayerInputComponent(class UInputComponent* InputComponent){
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AMyPlayer::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyPlayer::MoveRight);
	InputComponent->BindAction("Run", IE_Pressed, this, &AMyPlayer::StartRun);
	InputComponent->BindAction("Run", IE_Released, this, &AMyPlayer::StopRun);
	InputComponent->BindAction("Fire", IE_Pressed, this, &AMyPlayer::StartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &AMyPlayer::StopFire);
	InputComponent->BindAction("SwitchAssault", IE_Pressed, this, &AMyPlayer::SwitchToAssault);
	InputComponent->BindAction("SwitchLaser", IE_Pressed, this, &AMyPlayer::SwitchToLaser);
	InputComponent->BindAction("SwitchRocket", IE_Pressed, this, &AMyPlayer::SwitchToRocket);
}

void AMyPlayer::BeginPlay(){
	Super::BeginPlay();

	UMyGameInstance* MyGI = Cast<UMyGameInstance>(GetGameInstance());
	if(MyGI && MyGI->bShouldLoadSave){
		MyGI->bShouldLoadSave = false;
		UTutorialSaveGame* Instance = UTutorialSaveGame::CreateSaveInstance();
		Instance = Instance->Load(MyGI->LastSaveLoaded, 0);
		SetDataFromSave( Instance->SaveInfo );
	}
	else{
		HealthPoints = MaxHealthPoints;
	}
	HUDUpdateHP();
}

void AMyPlayer::Tick( float DeltaTime ){
	Super::Tick( DeltaTime );

	if(Controller && Controller->IsLocalController()){
		APlayerController* PC = Cast<APlayerController>(Controller);
		FVector MouseLocation, MouseDirection;
		PC->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
		SetActorRotation(FRotator(0, MouseDirection.Rotation().Yaw, 0));
		Bearing = MouseDirection.Rotation().Yaw;
	}

}

void AMyPlayer::MoveForward(float val){
	FVector Forward(1, 0, 0);
	AddMovementInput(Forward, val);
}

void AMyPlayer::MoveRight(float val){
	FVector Right(0, 1, 0);
	AddMovementInput(Right, val);
}

void AMyPlayer::StartRun(){
	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}
void AMyPlayer::StopRun(){
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AMyPlayer::StartFire(){
	if(Inventory.CurrentWeapon){
		Inventory.CurrentWeapon->StartFire();
	}
}
void AMyPlayer::StopFire(){
	if(Inventory.CurrentWeapon){
		Inventory.CurrentWeapon->StopFire();
	}
}

void AMyPlayer::SwitchToAssault(){
	if(Inventory.AssaultRifle){
		EquipWeapon(Inventory.AssaultRifle);
	}
}
void AMyPlayer::SwitchToLaser(){
	if(Inventory.LaserRifle){
		EquipWeapon(Inventory.LaserRifle);
	}
}
void AMyPlayer::SwitchToRocket(){
	if(Inventory.RocketLauncher){
		EquipWeapon(Inventory.RocketLauncher);
	}
}


float AMyPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser){
	//float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	float ActualDamage = 0.f;

	if(DamageEvent.IsOfType(FRadialDamageEvent::ClassID)){
		//reference to constant FRadialDamageEvent = Dataof(FRadialDamagePointerTo(AdressOf(DamageEvent)))
		const FRadialDamageEvent& RadDmg = *(FRadialDamageEvent*) &DamageEvent;

		float MinDamage = RadDmg.Params.MinimumDamage;
		float MaxDamage = RadDmg.Params.BaseDamage;
		float Distance = (GetActorLocation() - RadDmg.Origin).Size();
		float LerpConst = Distance/RadDmg.Params.OuterRadius;
		ActualDamage = FMath::Lerp(MaxDamage, MinDamage, LerpConst);
	}

	else /*if(DamageEvent.IsOfType(FPointDamageEvent::ClassID))*/{
		ActualDamage = DamageAmount;
	}

	HealthPoints = HealthPoints - ActualDamage;
	HUDUpdateHP();
	if(HealthPoints <= 0){
		OnDeath();
	}
	return ActualDamage;
}

void AMyPlayer::OnDeath(){
	UE_LOG(LogTemp, Warning, TEXT("You have died!"));
	Destroy();
}

void AMyPlayer::Heal(float Amount){
	if(Amount > 0){
		HealthPoints = HealthPoints + Amount;
		HUDUpdateHP();
	}
}

void AMyPlayer::AddAmmo(int32 Amount, EAmmoType AmmoType){
	switch(AmmoType){
		case EAmmoType::AT_Bullets:
			if(Inventory.AssaultRifle){
				Inventory.AssaultRifle->AddAmmo(Amount);
			}
			break;
		case EAmmoType::AT_Rockets:
			if(Inventory.RocketLauncher){
				Inventory.RocketLauncher->AddAmmo(Amount);
			}
			break;
		case EAmmoType::AT_Lasers:
			if(Inventory.LaserRifle){
				Inventory.LaserRifle->AddAmmo(Amount);
			}
			break;
		default:
			break;
	}
}

void AMyPlayer::AddToInventory(class AWeaponBase* NewWeapon){
	NewWeapon->SetCanInteract(false);
	NewWeapon->SetActorEnableCollision(false);
	NewWeapon->ChangeOwner(this);
	NewWeapon->AttachRootComponentTo(GetMesh(), WeaponSocketName, EAttachLocation::SnapToTarget);
	NewWeapon->SetActorHiddenInGame(true);

	//if weapon is AssaultRifleBase
	if(NewWeapon->IsA(AAssaultRifleBase::StaticClass())){
		if(Inventory.AssaultRifle){
			Inventory.AssaultRifle->Destroy();
		}
		Inventory.AssaultRifle = Cast<AAssaultRifleBase>(NewWeapon);

		if(!Inventory.CurrentWeapon || bEquipNewWeapon){
			EquipWeapon(Inventory.AssaultRifle);
		}
	}
	//if weapon is laser rifle
	else if(NewWeapon->IsA(ALaserRifleBase::StaticClass())){
		if(Inventory.LaserRifle){
			Inventory.LaserRifle->Destroy();
		}
		Inventory.LaserRifle = Cast<ALaserRifleBase>(NewWeapon);

		if(!Inventory.CurrentWeapon || bEquipNewWeapon){
			EquipWeapon(Inventory.LaserRifle);
		}
	}
	//weapon is rocket launcher
	else if(NewWeapon->IsA(ARocketLauncherBase::StaticClass())){
		if(Inventory.RocketLauncher){
			Inventory.RocketLauncher->Destroy();
		}
		Inventory.RocketLauncher = Cast<ARocketLauncherBase>(NewWeapon);

		if(!Inventory.CurrentWeapon || bEquipNewWeapon){
			EquipWeapon(Inventory.RocketLauncher);
		}
	}
}

void AMyPlayer::EquipWeapon(class AWeaponBase* WeaponToEquip){
	if(WeaponToEquip == Inventory.CurrentWeapon){
		return;
	}

	if(Inventory.CurrentWeapon){
		Inventory.CurrentWeapon->SetActorHiddenInGame(true);
	}

	if(WeaponToEquip == Inventory.AssaultRifle){
		Inventory.CurrentWeapon = Inventory.AssaultRifle;
	}
	else if(WeaponToEquip == Inventory.LaserRifle){
		Inventory.CurrentWeapon = Inventory.LaserRifle;
	}
	else if(WeaponToEquip == Inventory.RocketLauncher){
		Inventory.CurrentWeapon = Inventory.RocketLauncher;
	}

	Inventory.CurrentWeapon->SetActorHiddenInGame(false);
}

void AMyPlayer::SetHasKey(bool newHasKey){
	bHasKey = newHasKey;
	UE_LOG(LogTemp, Warning, TEXT("Has Key: %d"), bHasKey);
}
bool AMyPlayer::GetHasKey() const{
	return bHasKey;
}

void AMyPlayer::SetDataFromSave(const struct FSaveGameStruct& SaveData){
	HealthPoints = SaveData.Health;

	if(SaveData.AssaultRifle.Class){
		AWeaponBase* AssaultRifle = GetWorld()->SpawnActor<AWeaponBase>(SaveData.AssaultRifle.Class);
		AssaultRifle->bSpawnFull = false;
		AssaultRifle->SetAmmo(SaveData.AssaultRifle.Ammo);
		AddToInventory(AssaultRifle);
	}
	if(SaveData.LaserRifle.Class){
		AWeaponBase* LaserRifle = GetWorld()->SpawnActor<AWeaponBase>(SaveData.LaserRifle.Class);
		LaserRifle->bSpawnFull = false;
		LaserRifle->SetAmmo(SaveData.LaserRifle.Ammo);
		AddToInventory(LaserRifle);
	}
	if(SaveData.RocketLauncher.Class){
		AWeaponBase* RocketLauncher = GetWorld()->SpawnActor<AWeaponBase>(SaveData.RocketLauncher.Class);
		RocketLauncher->bSpawnFull = false;
		RocketLauncher->SetAmmo(SaveData.RocketLauncher.Ammo);
		AddToInventory(RocketLauncher);
	}

	switch(SaveData.CurrentWeapon){
		case 1: SwitchToAssault(); break;
		case 2: SwitchToLaser(); break;
		case 3: SwitchToRocket(); break;
		default: break;
	}
}

FSaveGameStruct AMyPlayer::GetDataForSave(){
	FSaveGameStruct SaveData;
	SaveData.Health = HealthPoints;
	SaveData.CurrentWeapon = Inventory.CurrentWeapon == Inventory.AssaultRifle ? 1 :
								Inventory.CurrentWeapon == Inventory.LaserRifle ? 2 :
								Inventory.CurrentWeapon == Inventory.RocketLauncher ? 3 : 0;

    if(Inventory.AssaultRifle){
		SaveData.AssaultRifle.Class = Inventory.AssaultRifle->GetClass();
		SaveData.AssaultRifle.Ammo = Inventory.AssaultRifle->GetCurrentAmmo();
	}
	if(Inventory.LaserRifle){
		SaveData.LaserRifle.Class = Inventory.LaserRifle->GetClass();
		SaveData.LaserRifle.Ammo = Inventory.LaserRifle->GetCurrentAmmo();
	}
	if(Inventory.RocketLauncher){
		SaveData.RocketLauncher.Class = Inventory.RocketLauncher->GetClass();
		SaveData.RocketLauncher.Ammo = Inventory.RocketLauncher->GetCurrentAmmo();
	}

	return SaveData;
}
