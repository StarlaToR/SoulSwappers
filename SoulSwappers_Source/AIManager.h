#pragma once
#include "CoreMinimal.h"
#include "MyAi.h"
#include "AIPool.h"
#include "ToolBox.h"
#include "MyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "AIManager.generated.h"


UCLASS()
class MAGIC_API AAIManager : public AAIPool
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		int numberOfBasicEnemies;

	UPROPERTY(EditAnywhere)
		int numberOfMediumEnemies;

	UPROPERTY(EditAnywhere)
		int numberOfLargeEnemies;

	UPROPERTY(EditAnywhere)
		TArray<AMyAI*> activePool;

	UPROPERTY(EditAnywhere)
		TArray<AMyAI*> passivePoolForBasic;

	UPROPERTY(EditAnywhere)
		TArray<AMyAI*> passivePoolForMedium;

	UPROPERTY(EditAnywhere)
		TArray<AMyAI*> passivePoolForLarge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AIM, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AMyAI> basicEnemyBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AIM, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AMyAI> mediumEnemyBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AIM, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AMyAI> largeEnemyBP;

	UFUNCTION()
		void CanDesactivate();

	UFUNCTION()
		void DesactivateAllActiveEnemies();
	
	bool binded;
	bool alreadyDesactivated;

public:	
	AAIManager();

	UFUNCTION()
		TArray<AMyAI*> GetBasicEnemies(int number);

	UFUNCTION()
		TArray<AMyAI*> GetMediumEnemies(int number);

	UFUNCTION()
		TArray<AMyAI*> GetLargeEnemies(int number);

	void DeadEnemyManagement(AActor* enemy) override;

	virtual void Tick(float deltaTime) override;

protected:
	virtual void BeginPlay() override;
};