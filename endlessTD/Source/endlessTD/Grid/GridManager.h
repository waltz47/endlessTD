// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include "GridManager.generated.h"

UCLASS()
class ENDLESSTD_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USceneComponent* SceneRoot;

	UPROPERTY()
		TArray<class AGridTile*> Tiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TSubclassOf<AGridTile> TileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		int32 GridXSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		int32 GridYSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		float TileXSpacing = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		float TileYSpacing = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		float OddRowOffset = 400.f;

	UFUNCTION(BlueprintCallable)
		FVector RotateVector(FVector Vec, float RotationAngle); 

	UFUNCTION(BlueprintCallable)
		void HandleClick();

	UPROPERTY()
		class ATurret* ActiveTurret;

	UFUNCTION()
		void SetFocusedTurret(ATurret* Turret);

	UFUNCTION(BlueprintImplementableEvent)
		void DoubleClickedTile(AActor* Actor);


};
