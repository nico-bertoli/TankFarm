#include "Enemy.h"
// #include "Components/SkeletalMeshComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	TArray<USkeletalMeshComponent*> skeletalMeshComponents;
	GetComponents<USkeletalMeshComponent>(skeletalMeshComponents);
	for (USkeletalMeshComponent* skeletalMeshComp : skeletalMeshComponents)
	{
		if (skeletalMeshComp)
		{
			if (skeletalMeshComp->ComponentHasTag(FName("Root")))
				root = skeletalMeshComp;
		}
	}
	
	ensure(root != nullptr);
	root->OnComponentHit.AddDynamic(this, &AEnemy::OnHit);
	
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
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Log, TEXT("hit"));
		Destroy();
	}
		
}

