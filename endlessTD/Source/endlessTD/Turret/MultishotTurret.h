// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Turret.h"
#include "MultishotTurret.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSTD_API AMultishotTurret : public ATurret
{
	GENERATED_BODY()

public:
	AMultishotTurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float CoverRadius = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float FireRate = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		int32 NumBullets = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		class UParticleSystem* MuzzleFlash;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		TSubclassOf<class AMissile> MissileClass;


	UFUNCTION(BlueprintCallable)
		void StartFiring();

	UFUNCTION(BlueprintCallable)
		void StopFiring();

	UFUNCTION(BlueprintCallable)
		void Fire();



	FTimerHandle FireHandle;

	virtual void Activate() override;
};
