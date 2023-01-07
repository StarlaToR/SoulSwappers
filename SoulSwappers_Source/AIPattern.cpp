#include "AIPattern.h"


AAIPattern::AAIPattern()
{
	PrimaryActorTick.bCanEverTick = true;

	numberOfBasicEnemiesNeeded = 1;
	numberOfMediumEnemiesNeeded = 0;
	numberOfLargeEnemiesNeeded = 0;

	numberOfRedBasicEnemiesWanted = 0;
	numberOfRedMediumEnemiesWanted = 0;
	numberOfRedLargeEnemiesWanted = 0;
}

void AAIPattern::BeginPlay()
{
	Super::BeginPlay();

	manager = Cast<AAIManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AAIManager::StaticClass()));
}

void AAIPattern::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void AAIPattern::Execute()
{
	basicEnemies = manager->GetBasicEnemies(numberOfBasicEnemiesNeeded);
	mediumEnemies = manager->GetMediumEnemies(numberOfMediumEnemiesNeeded);
	largeEnemies = manager->GetLargeEnemies(numberOfLargeEnemiesNeeded);

	float angle = 180 / (numberOfBasicEnemiesNeeded + 1);
	for (int i = 0; i < basicEnemies.Num(); i++)
	{
		FVector spawn = GetActorLocation() + (FVector::RightVector * 500).RotateAngleAxis(angle, FVector::ForwardVector);

		if (i < numberOfRedBasicEnemiesWanted)
			basicEnemies[i]->SetUp(spawn, ColorState::Red);
		else
			basicEnemies[i]->SetUp(spawn, ColorState::Blue);

		basicEnemies[i]->Activate();
		angle += 180 / (numberOfBasicEnemiesNeeded + 1);
	}

	angle = 180 / (numberOfMediumEnemiesNeeded + 1);
	for (int i = 0; i < mediumEnemies.Num(); i++)
	{
		FVector spawn = GetActorLocation() + (FVector::RightVector * 800).RotateAngleAxis(angle, FVector::ForwardVector);

		if (i < numberOfRedMediumEnemiesWanted)
			mediumEnemies[i]->SetUp(spawn, ColorState::Red);
		else
			mediumEnemies[i]->SetUp(spawn, ColorState::Blue);

		mediumEnemies[i]->Activate();
		angle += 180 / (numberOfMediumEnemiesNeeded + 1);
	}

	angle = 180 / (numberOfLargeEnemiesNeeded + 1);
	for (int i = 0; i < largeEnemies.Num(); i++)
	{
		FVector spawn = GetActorLocation() + (FVector::RightVector * 1100).RotateAngleAxis(angle, FVector::ForwardVector);

		if (i < numberOfRedLargeEnemiesWanted)
			largeEnemies[i]->SetUp(spawn, ColorState::Red);
		else
			largeEnemies[i]->SetUp(spawn, ColorState::Blue);

		largeEnemies[i]->Activate();
		angle += 180 / (numberOfLargeEnemiesNeeded + 1);
	}
}