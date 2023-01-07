#include "BowPattern.h"


void ABowPattern::Execute()
{
	if (monocolor)
		MonoColorBehaviour();
	else
		Behavior();
}

void ABowPattern::Behavior()
{
	basicEnemies = manager->GetBasicEnemies(numberOfBasicEnemiesNeeded);
	mediumEnemies = manager->GetMediumEnemies(numberOfMediumEnemiesNeeded);
	largeEnemies = manager->GetLargeEnemies(numberOfLargeEnemiesNeeded);

	bool isRed = false;
	float angle = 180 / (numberOfBasicEnemiesNeeded + 1);

	for (int i = 0; i < basicEnemies.Num(); i++)
	{
		FVector spawn = GetActorLocation() + (FVector::RightVector * 1000).RotateAngleAxis(angle, FVector::UpVector).RotateAngleAxis(-45, FVector::RightVector);

		if (isRed)
		{
			basicEnemies[i]->SetUp(spawn, ColorState::Red);
			isRed = false;
		}
		else
		{
			basicEnemies[i]->SetUp(spawn, ColorState::Blue);
			isRed = true;
		}

		basicEnemies[i]->Activate();
		angle += 180 / (numberOfBasicEnemiesNeeded + 1);
	}

	isRed = false;
	angle = 180 / (numberOfMediumEnemiesNeeded + 1);
	for (int i = 0; i < mediumEnemies.Num(); i++)
	{
		FVector spawn = GetActorLocation() + (FVector::RightVector * 1000).RotateAngleAxis(angle, FVector::UpVector).RotateAngleAxis(-45, FVector::RightVector);

		if (isRed)
		{
			mediumEnemies[i]->SetUp(spawn, ColorState::Red);
			isRed = false;
		}
		else
		{
			mediumEnemies[i]->SetUp(spawn, ColorState::Blue);
			isRed = true;
		}

		mediumEnemies[i]->Activate();
		angle += 180 / (numberOfMediumEnemiesNeeded + 1);
	}

	isRed = false;
	angle = 180 / (numberOfLargeEnemiesNeeded + 1);
	for (int i = 0; i < largeEnemies.Num(); i++)
	{
		FVector spawn = GetActorLocation() + (FVector::RightVector * 1000).RotateAngleAxis(angle, FVector::UpVector).RotateAngleAxis(-45, FVector::RightVector);

		if (isRed)
		{
			largeEnemies[i]->SetUp(spawn, ColorState::Red);
			isRed = false;
		}
		else
		{
			largeEnemies[i]->SetUp(spawn, ColorState::Blue);
			isRed = true;
		}

		largeEnemies[i]->Activate();
		angle += 180 / (numberOfLargeEnemiesNeeded + 1);
	}
}

void ABowPattern::MonoColorBehaviour()
{
	ColorState color = ColorState::Blue;
	if (numberOfRedBasicEnemiesWanted > 0)
		color = ColorState::Red;

	basicEnemies = manager->GetBasicEnemies(numberOfBasicEnemiesNeeded);
	mediumEnemies = manager->GetMediumEnemies(numberOfMediumEnemiesNeeded);
	largeEnemies = manager->GetLargeEnemies(numberOfLargeEnemiesNeeded);

	float angle = 180 / (numberOfBasicEnemiesNeeded + 1);
	for (int i = 0; i < basicEnemies.Num(); i++)
	{
		FVector spawn = GetActorLocation() + (FVector::RightVector * 1000).RotateAngleAxis(angle, FVector::UpVector).RotateAngleAxis(-45, FVector::RightVector);

		basicEnemies[i]->SetUp(spawn, color);

		basicEnemies[i]->Activate();
		angle += 180 / (numberOfBasicEnemiesNeeded + 1);
	}

	angle = 180 / (numberOfMediumEnemiesNeeded + 1);
	for (int i = 0; i < mediumEnemies.Num(); i++)
	{
		FVector spawn = GetActorLocation() + (FVector::RightVector * 1000).RotateAngleAxis(angle, FVector::UpVector).RotateAngleAxis(-45, FVector::RightVector);

		mediumEnemies[i]->SetUp(spawn, color);
		mediumEnemies[i]->Activate();
		angle += 180 / (numberOfMediumEnemiesNeeded + 1);
	}

	angle = 180 / (numberOfLargeEnemiesNeeded + 1);
	for (int i = 0; i < largeEnemies.Num(); i++)
	{
		FVector spawn = GetActorLocation() + (FVector::RightVector * 1000).RotateAngleAxis(angle, FVector::UpVector).RotateAngleAxis(-45, FVector::RightVector);

		largeEnemies[i]->SetUp(spawn, color);

		largeEnemies[i]->Activate();
		angle += 180 / (numberOfLargeEnemiesNeeded + 1);
	}
}