// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "GridTile.h"
#include "endlessTD/Turret/Turret.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	/*Spawn the Tile then copy to viewport 

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (int32 i = 0; i < GridXSize; i++) {
		for (int32 j = 0; j < GridYSize; j++) {
			float sx = TileXSpacing * (float)i;
			float sy = TileYSpacing * (float)j;
			if(i & 1) sy += OddRowOffset;
			FVector SpawnLocation(sx,sy,0.f);
			UWorld* World = GetWorld();
			if (World != nullptr) {
				AGridTile* Tile = World->SpawnActor<AGridTile>(TileClass, SpawnLocation, FRotator::ZeroRotator, SpawnParameters);
				Tiles.Add(Tile);
			}
		}
	}
	*/
	UWorld* World = GetWorld();
	if(World != nullptr) {
		TArray<AActor*> FActors;
		UGameplayStatics::GetAllActorsOfClass(World, TileClass, FActors);
		for(AActor* Actor:FActors) {
			Tiles.Add(Cast<AGridTile>(Actor));
		}
		UE_LOG(LogTemp, Warning, TEXT("Numer of tiles %d"), Tiles.Num());
	}
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AGridManager::RotateVector(FVector Vec, float RotationAngle)
{
	//Rotate a vector anticlockwise by rotation angle degrees
	float x = Vec.X, y = Vec.Y;
	FVector Ret;
	Ret.X = FMath::Cos(RotationAngle) * x - FMath::Sin(RotationAngle) * y;
	Ret.Y = FMath::Sin(RotationAngle) * x + FMath::Cos(RotationAngle) * y;
	return Ret;
}

void AGridManager::HandleClick()
{
	if(ActiveTurret == nullptr) {
		//UE_LOG(LogTemp, Warning, TEXT("Active Actor null"));
		return;

		UWorld* World = GetWorld();
		if (World == nullptr) {
			return;
		}
		APlayerController* PController = UGameplayStatics::GetPlayerController(World, 0);
		if (PController == nullptr) {
			return;
		}
		FVector StartLocation;
		FVector WorldDirection;
		PController->DeprojectMousePositionToWorld(StartLocation, WorldDirection);
		FHitResult Hit;
		World->LineTraceSingleByChannel(Hit, StartLocation, StartLocation + WorldDirection * 20000.f, ECollisionChannel::ECC_GameTraceChannel1);
		if (Hit.bBlockingHit) {
			AGridTile* Tile = Cast<AGridTile>(Hit.Actor);
			if (Tile != nullptr && !(Tile->Destroyed)) {
				if(Tile->DoubleClicked()) {
					DoubleClickedTile(Tile);
					return;
				}
			}
		}

		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("ClickFunction in Grid Manager"));
	ActiveTurret->PlaceAndActivate();
	ActiveTurret = nullptr;
}

void AGridManager::SetFocusedTurret(ATurret* Turret)
{
	if(Turret == nullptr) return;
	if(ActiveTurret != nullptr) ActiveTurret->PlaceAndActivate();
	ActiveTurret = Turret;
}

