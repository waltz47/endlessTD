// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Enemy.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if(World != nullptr) {
		FTimerHandle Diff;
		World->GetTimerManager().SetTimer(Diff, this, &AEnemySpawner::HandleDiffOverTime, 2.f, true, 0.f);
	}
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AEnemySpawner::SpawnSmall() {
	if(!Active) return;
	UWorld* World = GetWorld();
	if(World == nullptr) return;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z += 200.f;
	FRotator RandRot = UKismetMathLibrary::RandomRotator(false);
	RandRot.Pitch = 0.f;
	World->SpawnActor<AEnemy>(SmallZombieClass, SpawnLocation, RandRot, SpawnParameters);
}

void AEnemySpawner::SpawnLarge()
{
	if(!Active) return;
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z += 200.f;
	FRotator RandRot = UKismetMathLibrary::RandomRotator(false);
	RandRot.Pitch = 0.f;
	World->SpawnActor<AEnemy>(LargeZombieClass, SpawnLocation, RandRot, SpawnParameters);
}

void AEnemySpawner::SpawnHuge()
{
	if(!Active) return;
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z += 200.f;
	FRotator RandRot = UKismetMathLibrary::RandomRotator(false);
	RandRot.Pitch = 0.f;
	World->SpawnActor<AEnemy>(HugeZombieClass, SpawnLocation, RandRot, SpawnParameters);
}

void AEnemySpawner::SpawnCrawl()
{
	if(!Active) return;
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z += 200.f;
	FRotator RandRot = UKismetMathLibrary::RandomRotator(false);
	RandRot.Pitch = 0.f;
	World->SpawnActor<AEnemy>(CrawlZombieClass, SpawnLocation, RandRot, SpawnParameters);
}

void AEnemySpawner::HandleDiffOverTime()
{
	UWorld* World = GetWorld();
	float Seconds = World->GetTimeSeconds();
	const int Phase = Seconds / (PhaseNumSeconds);
	if(Phase > CurrentPhase) {
		//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, TEXT("Phase Incremented"));
		CurrentPhase++;
		if(CurrentPhase < SmallSpawnRate.Num() && SmallSpawnRate[CurrentPhase] != -1) {
			World->GetTimerManager().SetTimer(Small, this, &AEnemySpawner::SpawnSmall,SmallSpawnRate[CurrentPhase], true, 5.f);
		}
		if(CurrentPhase < LargeSpawnRate.Num() && LargeSpawnRate[CurrentPhase] != -1) {
			World->GetTimerManager().SetTimer(Large, this, &AEnemySpawner::SpawnLarge,LargeSpawnRate[CurrentPhase], true, 5.f);
		}
		if (CurrentPhase < HugeSpawnRate.Num() && HugeSpawnRate[CurrentPhase] != -1) {
			World->GetTimerManager().SetTimer(Huge, this, &AEnemySpawner::SpawnHuge, HugeSpawnRate[CurrentPhase], true, 5.f);
		}
		if (CurrentPhase < CrawlSpawnRate.Num() && CrawlSpawnRate[CurrentPhase] != -1) {
			World->GetTimerManager().SetTimer(Crawl, this, &AEnemySpawner::SpawnCrawl, CrawlSpawnRate[CurrentPhase], true, 5.f);
		}
	}
}
