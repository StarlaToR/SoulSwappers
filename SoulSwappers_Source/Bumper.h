#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Bumper.generated.h"


UCLASS()
class MAGIC_API ABumper : public AActor
{
	GENERATED_BODY()
	
public:	
	ABumper();

	UPROPERTY(EditAnywhere, Category = "Bumper")
		float BumpStrengh;
	
	UPROPERTY(EditAnywhere)
		FVector direction;

	UPROPERTY(EditAnywhere, Category = "Bumper")
		FName PlayerTag = "Player";

protected:
	UPROPERTY(EditAnywhere)
		UArrowComponent* arrow;

	UPROPERTY(EditAnywhere)
		UBoxComponent* boxCollider;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void BumperSound();

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	void BumperTarget(AActor* target);
	void ChangeArrowOrientation(FPropertyChangedEvent& e);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
#endif
};