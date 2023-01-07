#pragma once
#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "ColorStruct.h"
#include "MyAI.h"
#include "Bullet.generated.h"


UCLASS()
class MAGIC_API ABullet : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Life, meta = (AllowPrivateAccess = "true"))
		float timeLife;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Collider, meta = (AllowPrivateAccess = "true"))
		USphereComponent* sphereCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Shape, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* sphereMesh;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* trailRed;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* trailBlue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FX, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* trailFX;

	float currentTimeLife;
	float searchTargetDuration;

	AMyAI* aiToFollow;
	FVector direction;
	FColorStruct color;

public:	
	ABullet();

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

	virtual void Tick(float deltaTime) override;

protected:
	virtual void BeginPlay() override;
};