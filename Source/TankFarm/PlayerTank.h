#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerTank.generated.h"

class USpringArmComponent;
class AProjectile;

UCLASS()
class TANKFARM_API APlayerTank : public APawn
{
	GENERATED_BODY()
	//------------------------------------------------ Fields Settings
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true")) float moveForwardAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true")) float maxSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true")) float moveTurnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true")) float fasterTurnWhileMovingForwardMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true")) float slidingMultiplier;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true")) float forwardMovementBoostMultiplier = 2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true")) float jumpIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim", meta = (AllowPrivateAccess = "true")) FVector2D aimSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim", meta = (AllowPrivateAccess = "true")) float maxAimAngleY;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim", meta = (AllowPrivateAccess = "true")) float minAimAngleY;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomGravity", meta = (AllowPrivateAccess = "true")) float defaultGravity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomGravity", meta = (AllowPrivateAccess = "true")) float gravityWhileJumping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectile> currentBullet;

	//------------------------------------------------ Fields References
private:
	UStaticMeshComponent* root;
	UStaticMeshComponent* turret;
	UStaticMeshComponent* base;
	USpringArmComponent *cameraSpringArm;
	USceneComponent* spawnProjectilePosition;

	//------------------------------------------------ Fields
private:
	float lastTimeMovedForwardOrBackwards = -1;
	float lastTimePressedForwardMovementBoostButton = -1;
	float lastTimePressedJump = -1;
	float lastProjectileShotTime = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting", meta = (AllowPrivateAccess = "true")) bool IsTouchingGround;
	
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
	void HandleGravity() const;
	float GetCurrentGravity() const {return GetWorld()->GetTimeSeconds() - lastTimePressedJump < 0.1f ? gravityWhileJumping : defaultGravity;}

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
