#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "MysteryBox.generated.h"


UCLASS()
class MAGIC_API AMysteryBox : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		UBoxComponent* boxCollider;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere)
		float speedRotation;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float angle;
	
public:	
	AMysteryBox();

	virtual void Tick(float deltaTime) override;

protected:
	virtual void BeginPlay() override;
};