// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

UCLASS()
class ENDLESSTD_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USceneComponent* RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* BarrelMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* ActivationSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float VolumeMultiplier = 1.f;

	UPROPERTY(BlueprintReadWrite)
		bool Active = 0;

	UFUNCTION(BlueprintCallable)
		virtual void Activate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStatComponent* StatComponent;

	FTimerHandle PlaceHandle;

	UFUNCTION(BlueprintCallable)
		void MoveToMousePosition();

	UFUNCTION(BlueprintCallable)
		void PlaceAndActivate();

	UPROPERTY()
		class AGridManager* GridManager;

};
