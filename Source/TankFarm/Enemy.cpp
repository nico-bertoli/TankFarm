#include "Enemy.h"

#include "Components/CapsuleComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	root = FindComponentByClass<USkeletalMeshComponent>();
	collider = FindComponentByClass<UCapsuleComponent>();
	
	ensure(root != nullptr);
	ensure(collider != nullptr);
	
	collider->OnComponentHit.AddDynamic(this, &AEnemy::OnHit);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor->Tags.Contains("PlayerProjectile"))
	{
		Destroy();
	}
}

