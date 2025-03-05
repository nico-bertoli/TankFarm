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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") float moveForwardAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") float maxSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") float moveTurnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") float fasterTurnWhileMovingForwardMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") float slidingMultiplier;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") float forwardMovementBoostMultiplier = 2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") float jumpIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim") FVector2D aimSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim") float maxAimAngleY;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim") float minAimAngleY;

	//------------------------------------------------ Fields References
private:
	UStaticMeshComponent* root;
	UStaticMeshComponent* turret;
	UStaticMeshComponent* base;
	USpringArmComponent *cameraSpringArm;

	//------------------------------------------------ Fields Others
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (HideInEditor)) bool IsTouchingGround;
private:
	float lastTimeMovedForwardOrBackwards = -1;
	float lastTimePressedForwardMovementBoostButton = -1;
	float lastTimeJumped = -1;
	
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
	void Jump(float input);
	void ComputeSliding() const;

	//aim
	void AimX(float input);
	void AimY(float input);

	//fire
	void Fire(float input);

	// getters
	bool HasReachedMaxSpeedXY() const {return GetXYSpeed().Size() > GetCurrentMaxSpeed();}
	float GetCurrentMaxSpeed() const {return IsForwardBoostEnabled() ? maxSpeed * forwardMovementBoostMultiplier : maxSpeed;}
	bool IsForwardBoostEnabled() const {return GetWorld()->GetTimeSeconds() - lastTimePressedForwardMovementBoostButton < 0.1f;}
	FVector GetXYSpeed() const;
};
