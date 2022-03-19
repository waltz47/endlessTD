// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "endlessTD/StatComponent.h"
#include "GameFramework/Pawn.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//void AEnemy::CalculateAndApplyDamage(float BaseDamage)
//{
//	if(StatComponent == nullptr) {
//		UE_LOG(LogTemp, Warning, TEXT("No Stat Component On enemy"));
//		return;
//	}
//	//UE_LOG(LogTemp, Warning, TEXT("Enemy took %f Damage"), BaseDamage);
//	BaseDamage = StatComponent->CalculateAppliedDamage(BaseDamage);
//	float HP = StatComponent->HP;
//	HP -= BaseDamage;
//	StatComponent->HP = HP;
//	if(HP <= 0.f) {
//		Destroy();
//	}
//}