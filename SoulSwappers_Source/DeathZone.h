#pragma once
#include "CoreMinimal.h"
#include "GameFramework/KillZVolume.h"
#include "DeathZone.generated.h"


UCLASS()
class MAGIC_API ADeathZone : public AKillZVolume
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FName PlayerTag = "Player";

protected:
	virtual void ActorEnteredVolume(AActor* other) override;
};