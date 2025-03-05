// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TANKFARM_API AProjectile : public AActor
{
	GENERATED_BODY()
	
	//------------------------------------------------ Fields Settings
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") float shotImpulse;

	UStaticMeshComponent* root;

	//------------------------------------------------Methods
public:	
	AProjectile();
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void Shot(FVector startingPosition ,FVector direction);

};
