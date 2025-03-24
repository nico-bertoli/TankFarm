#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include "Projectile.generated.h"

// class UNiagaraSystem;

class UCapsuleComponent;

UCLASS()
class TANKFARM_API AProjectile : public AActor
{
	GENERATED_BODY()
	
	//------------------------------------------------ Fields
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true")) float shotImpulse;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true")) float rateoDelay;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* fxHit;
	
	UStaticMeshComponent* root;
	UCapsuleComponent* collider;

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
