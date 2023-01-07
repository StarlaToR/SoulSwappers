#include "AIPool.h"


AAIPool::AAIPool()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAIPool::BeginPlay()
{
	Super::BeginPlay();
}

void AAIPool::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}