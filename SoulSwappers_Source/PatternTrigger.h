#pragma once
#include "CoreMinimal.h"
#include "ToolBox.h"
#include "MyPlayer.h"
#include "AIPattern.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PatternTrigger.generated.h"

UCLASS()
class MAGIC_API APatternTrigger : public AActor
{
	GENERATED_BODY()

public:	
	APatternTrigger();

	UPROPERTY()
		bool hasBeenTriggered;

	UPROPERTY(EditAnywhere)
		UBoxComponent* hitbox;

	UPROPERTY(EditAnywhere)
		TArray<AAIPattern*> patterns;

	UFUNCTION()
		void triggerAvailable();

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Tick(float deltaTime) override;
	
protected:
	virtual void BeginPlay() override;
};
