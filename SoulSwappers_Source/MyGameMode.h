#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyPlayer.h"
#include "PlayerSpirit.h"
#include "MyGameMode.generated.h"


UCLASS()
class MAGIC_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BP, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AMyPlayer> myPlayerBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BP, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<APlayerSpirit> mySpiritBP;

	UFUNCTION()
		void SpawnPlayers();

protected:
	virtual void BeginPlay() override;
};