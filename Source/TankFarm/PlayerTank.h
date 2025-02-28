// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerTank.generated.h"

class USpringArmComponent;

UCLASS()
class TANKFARM_API APlayerTank : public APawn
{
	GENERATED_BODY()
	//------------------------------------------------ Fields Settings
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") float moveForwardSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") float moveTurnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") float fasterTurnWhileMovingForwardMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") FVector2D aimSpeed;

	//------------------------------------------------ Fields References
private:
	UStaticMeshComponent* turret;
	UStaticMeshComponent* base;
	USpringArmComponent *cameraSpringArm;

	//------------------------------------------------ Fields Others
	float lastTimeMovedForwardOrBackwards = -1;
	float lastTimePressedForwardMovementBoostButton = -1;
	
	float forwardMovementBoostMultiplier = 2;
	
	//------------------------------------------------Methods
public:
	APlayerTank();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	virtual void BeginPlay() override;

private:
	//move
	void MoveForward(float input);
	void ActivateMoveForwardBoost(float input);
	void MoveTurn(float input);

	//aim
	void AimX(float input);
	void AimY(float input);
};
