// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include "EnemySpawner.generated.h"

UCLASS()
class ENDLESSTD_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USceneComponent* RootScene = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PhaseNumSeconds = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AEnemy> SmallZombieClass;

	/*Put 4 Values -1 for no spawn*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> SmallSpawnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Active = 1;

	FTimerHandle Small;

	UFUNCTION(BlueprintCallable)
		void SpawnSmall();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AEnemy> LargeZombieClass;

	/*Put 4 Values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> LargeSpawnRate;

	FTimerHandle Large;

	UFUNCTION(BlueprintCallable)
		void SpawnLarge();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AEnemy> HugeZombieClass;

	/*Put 4 Values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> HugeSpawnRate;

	FTimerHandle Huge;

	UFUNCTION(BlueprintCallable)
		void SpawnHuge();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AEnemy> CrawlZombieClass;

	/*Put 4 Values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> CrawlSpawnRate;

	FTimerHandle Crawl;

	UFUNCTION(BlueprintCallable)
		void SpawnCrawl();



	UFUNCTION(BlueprintCallable)
		void HandleDiffOverTime();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CurrentPhase = 0;
	/*Increment every 30 seconds*/

};
