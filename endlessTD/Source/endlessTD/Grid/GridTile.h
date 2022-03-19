// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridTile.generated.h"

UCLASS()
class ENDLESSTD_API AGridTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* SceneRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* TileMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Destroyed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Occupied = nullptr;

	UFUNCTION(BlueprintCallable)
		bool CanBeUsed();

	UPROPERTY(BlueprintReadWrite)
		float LastClicked = -100.f;

	UFUNCTION(BlueprintCallable)
		bool DoubleClicked();



};
