// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	//meshes
	TArray<UStaticMeshComponent*> staticMeshComponents;
	GetComponents<UStaticMeshComponent>(staticMeshComponents);
	for (UStaticMeshComponent* meshComponent : staticMeshComponents)
	{
		if (meshComponent)
		{
			if (meshComponent->ComponentHasTag(FName("Root")))
				root = meshComponent;
		}
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRotation(root->GetPhysicsLinearVelocity().Rotation());
}

void AProjectile::Shot(FVector startingPosition, FVector direction)
{
	direction.Normalize();
	SetActorLocation(startingPosition);
	SetActorRotation(direction.Rotation());
	root->AddImpulse(shotImpulse * direction);
}


