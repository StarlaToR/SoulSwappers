#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIPool.generated.h"


UCLASS()
class MAGIC_API AAIPool : public AActor
{
	GENERATED_BODY()
	
public:	
	AAIPool();

	virtual void Tick(float deltaTime) override;
	virtual void DeadEnemyManagement(AActor* enemy) {};

protected:
	virtual void BeginPlay() override;
};