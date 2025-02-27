// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerTank.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
APlayerTank::APlayerTank()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerTank::BeginPlay()
{
	UE_LOG(LogTemp, Error, TEXT("BEGIN PLAY CALLED"));
	Super::BeginPlay();
}

void APlayerTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogTemp, Error, TEXT("SETUP CALLED"));

	//setup input
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerTank::MoveForward);
	PlayerInputComponent->BindAxis("AimX", this, &APlayerTank::AimX);
	PlayerInputComponent->BindAxis("AimY", this, &APlayerTank::AimY);

	//find turret
	TArray<UStaticMeshComponent*> staticMeshComponents;
	GetComponents<UStaticMeshComponent>(staticMeshComponents);
	for (UStaticMeshComponent* meshComponent : staticMeshComponents)
	{
		if (meshComponent && meshComponent->ComponentHasTag(FName("Turret")))
		{
			turret = meshComponent;
			break;
		}
	}
}

void APlayerTank::MoveForward(float input)
{
	float intensity = input * moveForwardSpeed;
	UE_LOG(LogTemp, Log, TEXT("moving forward input: %f, intensity: %f"),input,intensity );
	AddMovementInput(GetActorForwardVector(), intensity);
}

void APlayerTank::AimX(float input)
{
	float intensity = input * aimSpeed.X;
	UE_LOG(LogTemp, Log, TEXT("aiming x input: %f, intensity: %f"),input,intensity );
	FQuat rotation = FQuat(FRotator(0.0f, intensity, 0.0f));
	turret->AddLocalRotation(rotation); 
}

void APlayerTank::AimY(float input)
{
	float intensity = input * aimSpeed.Y;
	UE_LOG(LogTemp, Log, TEXT("aiming y input: %f, intensity: %f"),input,intensity );
	
}