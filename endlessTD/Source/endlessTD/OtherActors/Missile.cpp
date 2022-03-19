// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "endlessTD/Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AMissile::AMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollisionSphere"));
	SetRootComponent(CollisionComponent);

	MissileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MissileMesh"));
	MissileMesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMissile::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AMissile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Active == 0) return;
	if (TargetActor == nullptr || TargetActor->IsPendingKill() || !(TargetActor->IsValidLowLevel())) return;
	if ((GetActorLocation() - TargetActor->GetActorLocation()).Size() < 200.f) {
		Explode();
	}
}

void AMissile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!Active) return;
	if(OtherActor == nullptr) return;
	Explode();
}

void AMissile::Explode()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	if(ExplosionEmitter.Num() == 0) {
		return;
	}
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetActorLocation());

	SpawnTransform.SetScale3D(EmitterScale);
	UGameplayStatics::SpawnEmitterAtLocation(World, ExplosionEmitter[FMath::Rand() % ExplosionEmitter.Num()], SpawnTransform, true);

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	UClass* EnemyClass = AEnemy::StaticClass();
	TArray<AActor*> IgnoredActors;
	TArray<AActor*> OverlappingActors;
	UKismetSystemLibrary::SphereOverlapActors(World, GetActorLocation(), CoverRadius, TraceObjectTypes, EnemyClass, IgnoredActors, OverlappingActors);
	FVector Center = GetActorLocation();

	for(AActor* Actor:OverlappingActors) {
		if(Actor == nullptr) return;
		AEnemy* EnemyActor = Cast<AEnemy>(Actor);
		if(EnemyActor != nullptr) {
			//float Dist = (Center - EnemyActor->GetActorLocation()).Size();
			//Dist /= CoverRadius;
			UGameplayStatics::ApplyDamage(EnemyActor, Damage, nullptr, nullptr, UDamageType::StaticClass());
		}
	}
	Destroy();

}

void AMissile::Activate()
{
	if(Active == 1) return;
	if(TargetActor == nullptr) {
		Destroy();

	}
	ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
	ProjectileMovement->HomingAccelerationMagnitude = MoveSpeed;
	ProjectileMovement->bIsHomingProjectile = true;
	Active = 1;
}

