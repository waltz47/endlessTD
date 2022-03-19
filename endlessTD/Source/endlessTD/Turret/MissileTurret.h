// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Turret.h"
#include "MissileTurret.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSTD_API AMissileTurret : public ATurret
{
	GENERATED_BODY()

public:
	AMissileTurret();

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
		float CoverRadius = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float MinRadius = 1000.f;
	/*BPS*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float FireRate = 0.33f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		TSubclassOf<class AMissile> MissileClass;

	FTimerHandle TurretRotationHandle;

	UFUNCTION(BlueprintCallable)
		void StartFiring();

	UFUNCTION(BlueprintCallable)
		void StopFiring();

	UFUNCTION(BlueprintCallable)
		void Fire();

	FTimerHandle TurretFireHandle;

	UFUNCTION(BlueprintCallable)
		void FindTarget();
	
	UPROPERTY()
		AActor* TargetActor;

	virtual void Activate() override;

};
