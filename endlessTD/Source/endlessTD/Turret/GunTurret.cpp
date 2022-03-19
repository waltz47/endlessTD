// Fill out your copyright notice in the Description page of Project Settings.


#include "GunTurret.h"
#include "Engine/World.h"
#include "endlessTD/Enemy/Enemy.h"
#include "endlessTD/StatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"

AGunTurret::AGunTurret() {
	PrimaryActorTick.bCanEverTick = true;
}

void AGunTurret::BeginPlay()
{
	Super::BeginPlay();

}

void AGunTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Active == 0) return;
	if(TargetInRange(CurrentTarget)) {
		//UE_LOG(LogTemp, Warning, TEXT("Rotating Turret"));
		RotateTurret();
	}
}

void AGunTurret::Activate()
{
	Super::Activate();
	Active = 1;
	FTimerHandle NewTargetFindHandle;
	UWorld* World = GetWorld();
	if (World != nullptr) {
		World->GetTimerManager().SetTimer(NewTargetFindHandle, this, &AGunTurret::FindTarget, 2.f, true, 0.f);
	}
}


void AGunTurret::RotateTurret()
{
	if (CurrentTarget == nullptr || CurrentTarget->IsPendingKill() || !(CurrentTarget->IsValidLowLevel())) return;

	FRotator TurretRotation = BarrelMesh->GetComponentRotation();
	FRotator TargetRotation = (CurrentTarget->GetActorLocation() - BarrelMesh->GetComponentLocation()).Rotation();

	FRotator SetRotation = FMath::Lerp(TurretRotation, TargetRotation, TurretRotationRate);
	SetRotation.Roll = 0.f;
	SetRotation.Pitch = 0.f;
	BarrelMesh->SetWorldRotation(SetRotation);
}
bool AGunTurret::TargetInRange(AActor* Target) {
	if(Target == nullptr) return false;
	float Distance = (GetActorLocation() - Target->GetActorLocation()).Size();
	return Distance <= CoverRadius;
}

void AGunTurret::StartFiring()
{
	UWorld* World = GetWorld();
	if(World != nullptr) {
		bool Firing = World->GetTimerManager().IsTimerActive(TurretFireHandle);
		if(Firing == 0) {
			World->GetTimerManager().SetTimer(TurretFireHandle, this, &AGunTurret::Fire, 1.f/FireRate, true, 0.f);
		}
	}
}

void AGunTurret::StopFiring()
{
	UWorld* World = GetWorld();
	if(World != nullptr) {
		World->GetTimerManager().ClearTimer(TurretFireHandle);
	}
}

void AGunTurret::Fire()
{
	//UE_LOG(LogTemp, Warning, TEXT("Turret is Firing"));
	if(CurrentTarget == nullptr || CurrentTarget->IsPendingKill() || !(CurrentTarget->IsValidLowLevel())) {
		StopFiring();
		return;
	}
	UWorld* World = GetWorld();
	if(World != nullptr) {
		TArray<UActorComponent*> Tagged;
		Tagged = GetComponentsByTag(USceneComponent::StaticClass(), FName("GLaunch"));
		if (Tagged.Num() == 0) return;
		USceneComponent* Tmp = Cast<USceneComponent>(Tagged[0]);
		if(Tmp == nullptr) return;
		if(MuzzleFlash != nullptr) {
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(Tmp->GetComponentLocation());
			SpawnTransform.SetRotation(Tmp->GetComponentRotation().Quaternion());
			SpawnTransform.SetScale3D(FVector(2.f,2.f,2.f));
			UGameplayStatics::SpawnEmitterAtLocation(World, MuzzleFlash, SpawnTransform, true);
		}
		FVector StartLocation = Tmp->GetComponentLocation();
		FVector EndLocation = StartLocation + (Tmp->GetForwardVector() * CoverRadius);
		FHitResult OutHit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		FCollisionResponseParams RParams;
		World->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECollisionChannel::ECC_Pawn, Params, RParams);
		//DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, false, 100.f, (uint8)0U, 10.f);
		if(OutHit.bBlockingHit) {
			AEnemy* HitEnemy = Cast<AEnemy>(OutHit.Actor);
			if(HitEnemy != nullptr) {
				FDamageEvent Evt;
				if(StatComponent != nullptr) {
					UGameplayStatics::ApplyDamage(HitEnemy, StatComponent->GetDamage(), nullptr, nullptr, UDamageType::StaticClass());
				}
			}
		} else {
			//UE_LOG(LogTemp, Warning, TEXT("No Blocking Hit"));
		}
	}
}

void AGunTurret::FindTarget()
{
	if (CurrentTarget != nullptr && !(CurrentTarget->IsPendingKill()) && CurrentTarget->IsValidLowLevel()) return;
	UWorld* World = GetWorld();
	if(World == nullptr) return;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	UClass* EnemyClass = AEnemy::StaticClass();
	TArray<AActor*> IgnoredActors;
	TArray<AActor*> OverlappingActors;
	UKismetSystemLibrary::SphereOverlapActors(World, GetActorLocation(), CoverRadius, TraceObjectTypes, EnemyClass, IgnoredActors, OverlappingActors);
	for(AActor* Prospect : OverlappingActors) {
		if(Prospect == nullptr) continue;
		AEnemy* ProspectTarget = Cast<AEnemy>(Prospect);
		if(ProspectTarget != nullptr) {
			CurrentTarget = ProspectTarget;
			StartFiring();
			return;
		}
	}
}

