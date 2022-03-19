// Fill out your copyright notice in the Description page of Project Settings.


#include "MissileTurret.h"
#include "Engine/World.h"
#include "endlessTD/Enemy/Enemy.h"
#include "endlessTD/StatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "endlessTD/OtherActors/Missile.h"

AMissileTurret::AMissileTurret()
{
}

void AMissileTurret::BeginPlay()
{
	Super::BeginPlay();

}

void AMissileTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Active == 0) return;
	RotateTurret();
}

void AMissileTurret::Activate()
{
	Super::Activate();
	Active = 1;
	FTimerHandle NewTargetFindHandle;
	UWorld* World = GetWorld();
	if (World != nullptr) {
		World->GetTimerManager().SetTimer(NewTargetFindHandle, this, &AMissileTurret::FindTarget, 2.f, true, 0.f);
	}
}



void AMissileTurret::RotateTurret()
{
	if(TargetActor == nullptr) return;
	FRotator TurretRotation = BarrelMesh->GetComponentRotation();
	FRotator TargetRotation = (TargetActor->GetActorLocation() - BarrelMesh->GetComponentLocation()).Rotation();
	FVector Tmp = TargetRotation.Vector() * -1;
	FRotator SetRotation = FMath::Lerp(TurretRotation, Tmp.Rotation(), TurretRotationRate);
	SetRotation.Roll = 0.f;
	SetRotation.Pitch = 0.f;
	BarrelMesh->SetWorldRotation(SetRotation);
}

void AMissileTurret::StartFiring()
{
	UWorld* World = GetWorld();
	if(World != nullptr) {
		World->GetTimerManager().SetTimer(TurretFireHandle, this, &AMissileTurret::Fire, 1.f/FireRate, true, 0.f);
	}
}

void AMissileTurret::StopFiring()
{
	UWorld* World = GetWorld();
	if (World != nullptr) {
		World->GetTimerManager().ClearTimer(TurretFireHandle);
	}
}

void AMissileTurret::Fire()
{
	if(TargetActor == nullptr || TargetActor->IsPendingKill() ||  !(TargetActor->IsValidLowLevel())) {
		StopFiring();
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Target is valid %s"), *(TargetActor->GetName()));
	if((GetActorLocation() - TargetActor->GetActorLocation()).Size() < MinRadius) {
		TargetActor = nullptr;
		return;
	}
	UWorld* World = GetWorld();
	if(World != nullptr) {
		float SpawnOffset = 100.f;
		FVector StartLocation = BarrelMesh->GetComponentLocation();
		StartLocation += BarrelMesh->GetForwardVector() * SpawnOffset;
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		TArray<UActorComponent*> Tagged;
		Tagged = GetComponentsByTag(USceneComponent::StaticClass(), FName("MLaunch"));
		if(Tagged.Num() == 0) return;
		USceneComponent* Tmp = Cast<USceneComponent>(Tagged[0]);
		if(Tmp == nullptr) return;
		AMissile* Missile = World->SpawnActor<AMissile>(MissileClass, Tmp->GetComponentLocation(), Tmp->GetComponentRotation(), SpawnParameters);
		if(Missile != nullptr) {
			Missile->TargetActor = TargetActor;
			Missile->Activate();
		}
	}
}

void AMissileTurret::FindTarget()
{
	if(TargetActor != nullptr && !(TargetActor->IsPendingKill()) && TargetActor->IsValidLowLevel()) return;
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	UClass* EnemyClass = AEnemy::StaticClass();
	TArray<AActor*> IgnoredActors;
	TArray<AActor*> OverlappingActors;
	UKismetSystemLibrary::SphereOverlapActors(World, GetActorLocation(), CoverRadius, TraceObjectTypes, EnemyClass, IgnoredActors, OverlappingActors);
	for (AActor* Prospect : OverlappingActors) {
		if (Prospect == nullptr) continue;
		AEnemy* ProspectTarget = Cast<AEnemy>(Prospect);
		if (ProspectTarget != nullptr && (GetActorLocation() - ProspectTarget->GetActorLocation()).Size() >= MinRadius) {
			TargetActor = ProspectTarget;
			StartFiring();
			return;
		}
	}
}

