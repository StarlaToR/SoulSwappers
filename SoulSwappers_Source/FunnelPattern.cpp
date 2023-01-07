#include "FunnelPattern.h"


void AFunnelPattern::Execute()
{
	if (monocolor)
		MonoColorBehaviour();
	else
		Behavior();
}

void AFunnelPattern::Behavior()
{
	basicEnemies = manager->GetBasicEnemies(numberOfBasicEnemiesNeeded);
	mediumEnemies = manager->GetMediumEnemies(numberOfMediumEnemiesNeeded);
	largeEnemies = manager->GetLargeEnemies(numberOfLargeEnemiesNeeded);

	FVector rightModification = FVector::RightVector * 300;
	FVector leftModification = FVector::RightVector * (-300);
	FVector spawn = GetActorLocation();

	for (int i = 0; i < (int)(basicEnemies.Num() / 2); i++)
	{
		basicEnemies[i * 2]->SetUp(spawn + rightModification, ColorState::Red);
		basicEnemies[i * 2 + 1]->SetUp(spawn + leftModification, ColorState::Blue);

		basicEnemies[i * 2]->Activate();
		basicEnemies[i * 2 + 1]->Activate();

		rightModification += FVector::RightVector * 200;
		leftModification += FVector::RightVector * (-200);
		spawn += FVector::ForwardVector * (200) + FVector::UpVector * (50);
	}
}

void AFunnelPattern::MonoColorBehaviour()
{
	ColorState color = ColorState::Blue;
	if (numberOfRedBasicEnemiesWanted > 0)
		color = ColorState::Red;

	basicEnemies = manager->GetBasicEnemies(numberOfBasicEnemiesNeeded);
	mediumEnemies = manager->GetMediumEnemies(numberOfMediumEnemiesNeeded);
	largeEnemies = manager->GetLargeEnemies(numberOfLargeEnemiesNeeded);

	FVector rightModification = FVector::RightVector * 300;
	FVector leftModification = FVector::RightVector * (-300);
	FVector spawn = GetActorLocation();

	for (int i = 0; i < (int)(basicEnemies.Num() / 2); i++)
	{
		basicEnemies[i * 2]->SetUp(spawn + rightModification, color);
		basicEnemies[i * 2 + 1]->SetUp(spawn + leftModification, color);

		basicEnemies[i * 2]->Activate();
		basicEnemies[i * 2 + 1]->Activate();

		spawn += FVector::ForwardVector * (200) + FVector::UpVector * (50);
		rightModification += FVector::RightVector * 200;
		leftModification += FVector::RightVector * (-200);
	}
}