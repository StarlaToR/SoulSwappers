#pragma once
#include "CoreMinimal.h"
#include "MyAI.h"
#include "ToolBox.h"
#include "AIManager.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "AIPattern.generated.h"


UCLASS()
class MAGIC_API AAIPattern : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		TArray<AMyAI*> basicEnemies;

	UPROPERTY()
		TArray<AMyAI*> mediumEnemies;

	UPROPERTY()
		TArray<AMyAI*> largeEnemies;

	UPROPERTY(EditAnywhere)
		int numberOfBasicEnemiesNeeded;

	UPROPERTY(EditAnywhere)
		int numberOfMediumEnemiesNeeded;

	UPROPERTY(EditAnywhere)
		int numberOfLargeEnemiesNeeded;

	// if numberOfRedEnemiesWanted < numberOfEnemiesNeeded, the others enemies will spawn blue;
	UPROPERTY(EditAnywhere)
		int numberOfRedBasicEnemiesWanted;

	UPROPERTY(EditAnywhere)
		int numberOfRedMediumEnemiesWanted;

	UPROPERTY(EditAnywhere)
		int numberOfRedLargeEnemiesWanted;

	UPROPERTY(EditAnywhere)
		bool monocolor = false;

	UPROPERTY()
		AAIManager* manager;

public:	
	AAIPattern();

	virtual void Execute();
	virtual void Tick(float deltaTime) override;

protected:
	virtual void BeginPlay() override;
};