// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerTank.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include <Components/BoxComponent.h>

// Sets default values
APlayerTank::APlayerTank()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerTank::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//---------------------------- setup input
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//movement
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerTank::MoveForward);
	PlayerInputComponent->BindAxis("ActivateMoveForwardBoost", this, &APlayerTank::ActivateMoveForwardBoost);
	PlayerInputComponent->BindAxis("MoveTurn", this, &APlayerTank::MoveTurn);

	//aim
	PlayerInputComponent->BindAxis("AimX", this, &APlayerTank::AimX);
	PlayerInputComponent->BindAxis("AimY", this, &APlayerTank::AimY);
	PlayerInputComponent->BindAxis("Jump", this, &APlayerTank::Jump);
	

	//---------------------------- find components

	cameraSpringArm = FindComponentByClass<USpringArmComponent>();

	//meshes
	TArray<UStaticMeshComponent*> staticMeshComponents;
	GetComponents<UStaticMeshComponent>(staticMeshComponents);
	for (UStaticMeshComponent* meshComponent : staticMeshComponents)
	{
		if (meshComponent)
		{
			if (meshComponent->ComponentHasTag(FName("Root")))
				root = meshComponent;
			if(meshComponent->ComponentHasTag(FName("Turret")))
				turret = meshComponent;
			if(meshComponent->ComponentHasTag(FName("Base")))
				base = meshComponent;
			
		}
	}
}

void APlayerTank::MoveForward(float input)
{
	if (IsTouchingGround == false)
		return;

	float intensity = input * moveForwardSpeed;

	//apply forward boost
	if(GetWorld()->GetTimeSeconds() - lastTimePressedForwardMovementBoostButton < 0.1f)
		intensity *= forwardMovementBoostMultiplier;
	
	UE_LOG(LogTemp, Display, TEXT("adding forward force: %f"),intensity);
	root->AddForce(base->GetForwardVector() * intensity);

	if(input!=0)
		lastTimeMovedForwardOrBackwards = GetWorld()->GetTimeSeconds();
}

void APlayerTank::ActivateMoveForwardBoost(float input)
{
	if(input != 0.0f)
		lastTimePressedForwardMovementBoostButton = GetWorld()->GetTimeSeconds();
}

void APlayerTank::MoveTurn(float input)
{
	float intensity = input * moveTurnSpeed;

	//faster turn when moving forward
	if(GetWorld()->GetTimeSeconds() - lastTimeMovedForwardOrBackwards < 0.1)
		intensity *= fasterTurnWhileMovingForwardMultiplier;
	
	FQuat rotation = FQuat(FRotator(0.0f, intensity, 0.0f));
	base->AddLocalRotation(rotation); 
}

void APlayerTank::Jump(float input)
{
	//bool canJump = GetWorld()->GetTimeSeconds() - lastTimeJumped > 0.1f;
	if (input != 0 && IsTouchingGround)
	{
		base->AddImpulse(FVector(0, 0, jumpIntensity), NAME_None, true);
		lastTimeJumped = GetWorld()->GetTimeSeconds();
	}
}


void APlayerTank::AimX(float input)
{
	float intensity = input * aimSpeed.X;
	FQuat rotation = FQuat(FRotator(0.0f, intensity, 0.0f));
	turret->AddLocalRotation(rotation); 
}

void APlayerTank::AimY(float input)
{
	float intensity = input * aimSpeed.Y;
	FRotator deltaRotation(intensity, 0.0f, 0.0f);
	FRotator newRotation = cameraSpringArm->GetRelativeRotation() + deltaRotation;

	// clamp new rotation
	newRotation.Pitch = FMath::Clamp(newRotation.Pitch, minAimAngleY, maxAimAngleY);

	cameraSpringArm->SetRelativeRotation(newRotation);
}

