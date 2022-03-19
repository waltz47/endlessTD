// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include "Missile.generated.h"

UCLASS()
class ENDLESSTD_API AMissile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* CollisionComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* MissileMesh = nullptr;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void Explode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* TargetActor = nullptr;

	UPROPERTY(EditAnywhere, BLueprintReadWrite)
		class UProjectileMovementComponent* ProjectileMovement = nullptr;

	/*Call after setting target actor*/
	UFUNCTION(BlueprintCallable)
		void Activate(); 

	UPROPERTY()
		bool Active = 0;

	/*Homing magnitude*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
		float MoveSpeed = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
		float CoverRadius = 800.f;

	/*This is the base damage. Applied dmg will fall off with distance*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
		float Damage = 50.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
		TArray<class  UParticleSystem*> ExplosionEmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
		FVector EmitterScale;

};
