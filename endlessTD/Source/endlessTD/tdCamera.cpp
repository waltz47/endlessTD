// Fill out your copyright notice in the Description page of Project Settings.


#include "tdCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Grid/GridManager.h"
// Sets default values
AtdCamera::AtdCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	SetRootComponent(RootScene);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	SpringArm->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void AtdCamera::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	AActor* Actor = UGameplayStatics::GetActorOfClass(World, AGridManager::StaticClass());
	if(Actor != nullptr) {
		GridManager = Cast<AGridManager>(Actor);
		if(GridManager != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Grid Manager Found"));
		}
	}
}

// Called every frame
void AtdCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AtdCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("HMove", this, &AtdCamera::HMovement);
	PlayerInputComponent->BindAxis("VMove", this, &AtdCamera::VMovement);

	PlayerInputComponent->BindAction("LClick", EInputEvent::IE_Released, this, &AtdCamera::LeftClickFunction);
}
void AtdCamera::VMovement(float AxisValue) {
	//UE_LOG(LogTemp, Warning, TEXT("Axis is %f:"), AxisValue);
	AddActorWorldOffset(FVector(AxisValue*MoveSpeed, 0.f, 0.f));
}
void AtdCamera::LeftClickFunction()
{
	if(GridManager == nullptr) return;
	GridManager->HandleClick();
}
void AtdCamera::IncrementKill()
{
	Kills++;
	if(Kills % PointScoring == 0) {
		Points++;
	}
}
void AtdCamera::HMovement(float AxisValue) {
	AddActorWorldOffset(FVector(0.f, AxisValue*MoveSpeed, 0.f));
}

