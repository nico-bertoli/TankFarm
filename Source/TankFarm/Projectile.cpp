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
	if(OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("bullet hit!"));
		
		UNiagaraComponent* NiagaraEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation
		(
			GetWorld(),    
			fxHit,          
			GetActorLocation(),
			GetActorRotation(),
			FVector(1,1,1),
			true, 
			true 
		);
		
		Destroy();
	}
		
}



