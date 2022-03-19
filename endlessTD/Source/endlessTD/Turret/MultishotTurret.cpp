// Fill out your copyright notice in the Description page of Project Settings.


#include "MultishotTurret.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "endlessTD/Enemy/Enemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "endlessTD/OtherActors/Missile.h"

AMultishotTurret::AMultishotTurret()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMultishotTurret::BeginPlay()
{
	Super::BeginPlay();
}

void AMultishotTurret::Activate()
{
	Super::Activate();
	Active = 1;
	StartFiring();
}


void AMultishotTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMultishotTurret::StartFiring()
{
	UWorld* World = GetWorld();
	if(World != nullptr) {
		World->GetTimerManager().SetTimer(FireHandle, this, &AMultishotTurret::Fire, 1.f/FireRate, true, 0.f);
	}
}

void AMultishotTurret::StopFiring()
{
	UWorld* World = GetWorld();
	if (World != nullptr) {
		World->GetTimerManager().ClearTimer(FireHandle);
	}
}

void AMultishotTurret::Fire()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	for(int32 i=0;i<NumBullets;i++) {
		float Angle = 360.f / NumBullets;
		Angle *= (float)i;
		FRotator TmpRot(0.f, Angle, 0.f);
		FVector Fwd = FRotationMatrix(TmpRot).GetScaledAxis(EAxis::X);
		//Fwd is forward vector for spawning projectile
		//spawn here
		FVector StartLocation = GetActorLocation();
		FVector EndLocation = StartLocation + Fwd * CoverRadius;
		FHitResult OutHit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		FCollisionResponseParams RParams;
		World->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECollisionChannel::ECC_Pawn, Params, RParams);
		//DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, false, 100.f, (uint8)0U, 10.f);
		if (OutHit.bBlockingHit) {
			AEnemy* HitEnemy = Cast<AEnemy>(OutHit.Actor);
			if (HitEnemy != nullptr) {
				FActorSpawnParameters SpawnParameters;
				SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				AMissile* Missile = World->SpawnActor<AMissile>(MissileClass, GetActorLocation() + FVector(0.f,0.f, 100.f) + Fwd * 50.f, Fwd.Rotation(), SpawnParameters);
				if (Missile != nullptr) {
					Missile->TargetActor = HitEnemy;
					Missile->Activate();
					//if (MuzzleFlash != nullptr) {
					//	FTransform SpawnTransform;
					//	SpawnTransform.SetLocation((GetActorLocation() + Fwd * 50.f) + FVector(0.f, 0.f, 50.f));
					//	SpawnTransform.SetRotation(Fwd.ToOrientationQuat());
					//	SpawnTransform.SetScale3D(FVector(1.f, 1.f, 1.f));
					//	UGameplayStatics::SpawnEmitterAtLocation(World, MuzzleFlash, SpawnTransform, true);
					//}
				}
			}
		}

	}
}

