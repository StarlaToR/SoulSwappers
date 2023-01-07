#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PhysicsVolume.h"
#include "Checkpoint.generated.h"


UCLASS()
class MAGIC_API ACheckpoint : public APhysicsVolume
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FName PlayerTag = "Player";

protected:
	virtual void ActorEnteredVolume(AActor* other) override;
};