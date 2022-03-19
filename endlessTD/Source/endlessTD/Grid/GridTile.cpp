// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"	
// Sets default values
AGridTile::AGridTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	TileMesh->SetupAttachment(SceneRoot);



}

// Called when the game starts or when spawned
void AGridTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGridTile::CanBeUsed()
{
	if(Destroyed) return false;
	if(Occupied == nullptr || Occupied->IsPendingKill() || !(Occupied->IsValidLowLevel())) return true;
	return false;
}

bool AGridTile::DoubleClicked()
{
	if(Destroyed) return false;
	UWorld* World = GetWorld();
	if(World == nullptr) return false;
	const float ClickTime = World->GetTimeSeconds();
	//UE_LOG(LogTemp, Warning, TEXT("Click Diff %f"), abs(ClickTime-LastClicked));
	if(abs(ClickTime - LastClicked) <= 0.3f) {
		LastClicked = ClickTime;
		return true;
	}
	LastClicked = ClickTime;
	return false;
}



