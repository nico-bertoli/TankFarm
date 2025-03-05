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

	if(input!=0)
		lastTimeMovedForwardOrBackwards = GetWorld()->GetTimeSeconds();

	float intensity = input * moveForwardAcceleration;
	
	if(IsForwardBoostEnabled())
		intensity *= forwardMovementBoostMultiplier;
	
	if(HasReachedMaxSpeedXY() == false)
		root->AddImpulse(base->GetForwardVector() * intensity);	//todo would be better to use add force but it's not working for some reason
	
	ComputeSliding();
}

void APlayerTank::ComputeSliding() const
{
	FVector forwardDirection = base->GetForwardVector();
	FVector xySpeed = GetXYSpeed();  // Replace WheelComponent with your actual component
	FVector forwardSpeed = FVector::DotProduct(xySpeed, forwardDirection) * forwardDirection;
	FVector sideSpeed = xySpeed - forwardSpeed;  // This is the velocity that's not aligned with forward
	
	UE_LOG(LogTemp, Warning, TEXT("side speed: %f"),sideSpeed.Size());
	
	sideSpeed *= slidingMultiplier;
	FVector computedSlidingSpeed = forwardSpeed + sideSpeed;
	root->SetPhysicsLinearVelocity(FVector(computedSlidingSpeed.X,computedSlidingSpeed.Y,root->GetPhysicsLinearVelocity().Z));
}

FVector APlayerTank::GetXYSpeed() const
{
	FVector xySpeed = root->GetPhysicsLinearVelocity();
	xySpeed.Z = 0;
	return xySpeed;
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
		root->AddImpulse(FVector(0, 0, jumpIntensity), NAME_None, true);
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

