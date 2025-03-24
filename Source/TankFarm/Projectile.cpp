#include "Projectile.h"

#include "Components/CapsuleComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	root = FindComponentByClass<UStaticMeshComponent>();
	collider = FindComponentByClass<UCapsuleComponent>();
	
	ensure(root != nullptr);
	ensure(collider != nullptr);
	
	collider->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
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
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("bullet hit!"));
		
		// GetWorld()->SpawnActor<AProjectile>
		// (
		// 	fxHit,
		// 	GetActorLocation(),
		// 	GetActorRotation()
		// );

		UNiagaraComponent* NiagaraEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation
		(
			GetWorld(),     // The world context
			fxHit,          // Niagara system asset (UNiagaraSystem*)
			GetActorLocation(), // The location where the effect should spawn
			GetActorRotation(), // The rotation of the effect
			FVector(1,1,1),
			true,           // Whether to auto destroy the effect when it's done
			true            // Whether the effect should scale with the world
		);
		
		Destroy();
	}
		
}



