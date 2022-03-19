// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ENDLESSTD_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HP = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHP = 100.f;

	/*Slows down fire rate? Reduce armor? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Corruption = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Armor = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage = 10.f;

	UFUNCTION(BlueprintCallable)
		float CalculateAppliedDamage(float BaseDamage);

	UFUNCTION(BlueprintCallable)
		float GetDamage();

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float BaseDamage, const class UDamageType* DamageType, class AController* Instigator, AActor* Causer);

	UFUNCTION(BlueprintImplementableEvent)
		void Died();

};
