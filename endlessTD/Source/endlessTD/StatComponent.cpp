// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"
#include "endlessTD/Enemy/Enemy.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "endlessTD/tdCamera.h"
// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	HP = MaxHP;
	Corruption = 0.f;
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if(Owner != nullptr) {
		Owner->OnTakeAnyDamage.AddDynamic(this, &UStatComponent::TakeDamage);
	}
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UStatComponent::CalculateAppliedDamage(float BaseDamage)
{
	float RdDmg = BaseDamage / (5.f + Armor/100.f);
	if(RdDmg < 0.f) RdDmg = 0.f;
	return RdDmg;
}

float UStatComponent::GetDamage()
{
	return Damage * ((100.f - Corruption) / 100.f);
}

void UStatComponent::TakeDamage(AActor* DamagedActor, float BaseDamage, const UDamageType* DamageType, AController* Instigator, AActor* Causer)
{
	float AppliedDmg = CalculateAppliedDamage(BaseDamage);
	HP -= AppliedDmg;
	if(HP <= 0) {
		AActor* Owner = GetOwner();
		if(Owner != nullptr) {
			AEnemy* Tmp = Cast<AEnemy>(Owner);
			if(Tmp != nullptr) {
				//Kill ++ 
				UWorld* World = GetWorld();
				if(World != nullptr) {
					APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
					if(PlayerPawn != nullptr) {
						AtdCamera* Player = Cast<AtdCamera>(PlayerPawn);
						if(Player != nullptr) {
							Player->IncrementKill();
						}
					}
				}
			}
			Owner->Destroy();
		}
	}
}

