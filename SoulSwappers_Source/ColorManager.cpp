#include "ColorManager.h"
#include "ToolBox.h"


AColorManager::AColorManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AColorManager::BeginPlay()
{
	Super::BeginPlay();
}

void AColorManager::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void AColorManager::ChanceColorActive(bool isRed)
{
	if (isRed)
	{
		blueStateDelegate.Broadcast(false);
		redStateDelegate.Broadcast(true);
	}
	else
	{
		blueStateDelegate.Broadcast(true);
		redStateDelegate.Broadcast(false);
	}
}