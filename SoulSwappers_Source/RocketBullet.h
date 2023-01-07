#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "ColorStruct.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "MyAI.h"
#include "RocketBullet.generated.h"


UCLASS()
class MAGIC_API ARocketBullet : public AActor
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Life, meta = (AllowPrivateAccess = "true"))
		float timeLife;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float radiusExplosion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float searchTargetDuration = 1.5f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Collider, meta = (AllowPrivateAccess = "true"))
		USphereComponent* sphereCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Shape, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* sphereMesh;

	float currentTimeLife;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* trailFX;
	UPROPERTY(EditAnywhere, Category = "FX")
		FLinearColor blueColor;
	UPROPERTY(EditAnywhere, Category = "FX")
		FLinearColor redColor;

	UPROPERTY(EditAnywhere, Category = "FX")
		class UNiagaraSystem* explosionFX;
	FVector direction;
	AMyAI* aiToFollow;
	FColorStruct color;

public:
	ARocketBullet();

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void IncreaseSize();
	void ActiveCollision();
	void FindValidTarget();
	void SetSpeed(float newSpeed);
	void SetDirection(FVector dir);
	void SetPawnToFollow(AMyAI* pawn);
	void SetColor(ColorState newColor);
	void SetTimeLife(float newTimeLife);

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};