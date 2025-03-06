#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TANKFARM_API AProjectile : public AActor
{
	GENERATED_BODY()
	
	//------------------------------------------------ Fields
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true")) float shotImpulse;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true")) float rateoDelay;
	
	UStaticMeshComponent* root;

	//------------------------------------------------Methods
public:	
	AProjectile();
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void Shot();
	float GetRateoDelay()const {return rateoDelay;}
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
