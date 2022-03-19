// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Turret.h"
#include "GunTurret.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSTD_API AGunTurret : public ATurret
{
	GENERATED_BODY()

public:
	AGunTurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	
	UFUNCTION(BlueprintCallable)
		void RotateTurret();

	UPROPERTY(BlueprintReadWrite)
		AActor* CurrentTarget = nullptr;

	/*Rate of rotation of turret (0-1) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float TurretRotationRate = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float CoverRadius = 1500.f;

	/*BPS*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float FireRate = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		class UParticleSystem* MuzzleFlash;

	FTimerHandle TurretRotationHandle;
	UFUNCTION(BlueprintCallable)
		bool TargetInRange(AActor* Target);

	UFUNCTION(BlueprintCallable)
		void StartFiring();

	UFUNCTION(BlueprintCallable)
		void StopFiring();

	UFUNCTION(BlueprintCallable)
		void Fire();

	FTimerHandle TurretFireHandle;

	UFUNCTION(BlueprintCallable)
		void FindTarget();

	virtual void Activate() override;
};
