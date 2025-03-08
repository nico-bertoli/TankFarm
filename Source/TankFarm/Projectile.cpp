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
	
	ensure(root != nullptr);
	root->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRotation(root->GetPhysicsLinearVelocity().Rotation());
}

void AProjectile::Shot()
{
	root->AddImpulse(shotImpulse * GetActorForwardVector());
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("hit"));
	if (OtherActor && OtherActor != this)
		Destroy();
}



