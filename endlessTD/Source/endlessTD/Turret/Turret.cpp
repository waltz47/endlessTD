// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components/StaticMeshComponent.h"
#include "endlessTD/StatComponent.h"
#include "GameFramework/PlayerController.h"
#include "endlessTD/Grid/GridManager.h"
#include "endlessTD/Grid/GridTile.h"
#include "endlessTD/tdCamera.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Sound/SoundBase.h"
#include "Engine/World.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = RootScene;
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(RootComponent);

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	BarrelMesh->SetupAttachment(TurretMesh);

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if(World != nullptr) {
		AActor* GM = UGameplayStatics::GetActorOfClass(World, AGridManager::StaticClass());
		if(GM != nullptr) GridManager = Cast<AGridManager>(GM);
		if(GridManager != nullptr) {
			//UE_LOG(LogTemp, Warning, TEXT("Setting Focused turret in GM"));
			GridManager->SetFocusedTurret(this);
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Invalid GManager in Turret CPP"));
		}
		World->GetTimerManager().SetTimer(PlaceHandle, this, &ATurret::MoveToMousePosition, 0.1f, true);
	}
	
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ATurret::Activate()
{
	Active = 1;
}
void ATurret::MoveToMousePosition() {
	UWorld* World = GetWorld();
	if(World == nullptr) {
		Destroy();
		return;
	}
	APlayerController* PController = UGameplayStatics::GetPlayerController(World, 0);
	if(PController == nullptr) {
		return;
	}
	FVector StartLocation;
	FVector WorldDirection;
	PController->DeprojectMousePositionToWorld(StartLocation, WorldDirection);
	//TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	//TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	//FHitResult Hit;
	//PController->GetHitResultUnderCursorForObjects(TraceObjectTypes, false, Hit);
	FHitResult Hit;
	World->LineTraceSingleByChannel(Hit, StartLocation, StartLocation + WorldDirection * 20000.f, ECollisionChannel::ECC_GameTraceChannel1);
	if(Hit.bBlockingHit) {
		AGridTile* Tile = Cast<AGridTile>(Hit.Actor);
		if(Tile != nullptr && Tile->CanBeUsed()) {
			SetActorLocation(Tile->GetActorLocation());
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("No valid Grid tile"));
	}
}

void ATurret::PlaceAndActivate()
{
	if(Active == 1) return;
	bool placed = 0;
	UWorld* World = GetWorld();
	if (World == nullptr) {
		Destroy();
		return;
	}
	//World->GetTimerManager().ClearTimer(PlaceHandle);
	//APlayerController* PController = UGameplayStatics::GetPlayerController(World, 0);
	//TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	//TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	//FHitResult Hit;
	//PController->GetHitResultUnderCursorForObjects(TraceObjectTypes, false, Hit);
	//if (Hit.bBlockingHit) {
	//	AGridTile* Tile = Cast<AGridTile>(Hit.Actor);
	//	if (Tile != nullptr) {
	//		SetActorLocation(Tile->GetActorLocation());
	//		placed = 1;
	//	}
	//}
	APlayerController* PController = UGameplayStatics::GetPlayerController(World, 0);
	if (PController == nullptr) {
		return;
	}
	World->GetTimerManager().ClearTimer(PlaceHandle);
	FVector StartLocation;
	FVector WorldDirection;
	PController->DeprojectMousePositionToWorld(StartLocation, WorldDirection);
	FHitResult Hit;
	World->LineTraceSingleByChannel(Hit, StartLocation, StartLocation + WorldDirection * 20000.f, ECollisionChannel::ECC_GameTraceChannel1);
	if (Hit.bBlockingHit) {
		AGridTile* Tile = Cast<AGridTile>(Hit.Actor);
		if (Tile != nullptr && Tile->CanBeUsed()) {
			SetActorLocation(Tile->GetActorLocation());
			Tile->Occupied = this;
			placed = 1;
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("No Tile On Trace From Screen"));
	}
	if(!placed) {
		Destroy();
	} else {
		if(ActivationSound != nullptr) {
			UGameplayStatics::PlaySoundAtLocation(World, ActivationSound, GetActorLocation(), VolumeMultiplier);
		}
		Activate();
	}
}

