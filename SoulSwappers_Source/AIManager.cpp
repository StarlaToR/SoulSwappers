#include "AIManager.h"


AAIManager::AAIManager()
{
	PrimaryActorTick.bCanEverTick = true;

	binded = false;
	alreadyDesactivated = false;

	numberOfBasicEnemies = 20;
	numberOfMediumEnemies = 15;
	numberOfLargeEnemies = 10;
}

void AAIManager::BeginPlay()
{
	Super::BeginPlay();

	FVector Location(0.f, 0.f, -1000.f);
	FRotator Rotation(0.f, 0.f, 0.f);
	FActorSpawnParameters SpawnInfo;

	for (int i = 0; i < numberOfBasicEnemies; i++)
	{
		AActor* spawned = GetWorld()->SpawnActor<AActor>(basicEnemyBP, Location, Rotation, SpawnInfo);
		AMyAI* enemy = Cast<AMyAI>(spawned);

		enemy->pool = this;
		passivePoolForBasic.Add(enemy);

		if (Location.X > -1000)
		{
			Location.X -= 100;
		}
		else
		{
			Location.Y -= 100;
			Location.X = 0;
		}
	}

	for (int i = 0; i < numberOfMediumEnemies; i++)
	{
		AActor* spawned = GetWorld()->SpawnActor<AActor>(mediumEnemyBP, Location, Rotation, SpawnInfo);
		AMyAI* enemy = Cast<AMyAI>(spawned);

		enemy->pool = this;
		passivePoolForMedium.Add(enemy);

		if (Location.X > -1000)
		{
			Location.X -= 100;
		}
		else
		{
			Location.Y -= 100;
			Location.X = 0;
		}
	}

	for (int i = 0; i < numberOfLargeEnemies; i++)
	{
		AActor* spawned = GetWorld()->SpawnActor<AActor>(largeEnemyBP, Location, Rotation, SpawnInfo);
		AMyAI* enemy = Cast<AMyAI>(spawned);

		enemy->pool = this;
		passivePoolForLarge.Add(enemy);

		if (Location.X > -1000)
		{
			Location.X -= 100;
		}
		else
		{
			Location.Y -= 100;
			Location.X = 0;
		}
	}
}

void AAIManager::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (!binded)
	{
		ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (character != nullptr)
		{
			AMyPlayer* player = Cast<AMyPlayer>(character);
			player->isDead.AddDynamic(this, &AAIManager::DesactivateAllActiveEnemies);
			binded = true;
		}
	}
}

TArray<AMyAI*> AAIManager::GetBasicEnemies(int number)
{
	TArray<AMyAI*> array;

	if (number > passivePoolForBasic.Num())
		return array;

	for (int i = 0; i < number; i++)
	{
		AMyAI* enemy = passivePoolForBasic[0];
		passivePoolForBasic.Remove(enemy);
		activePool.Add(enemy);
		array.Add(enemy);
	}

	return array;
}

TArray<AMyAI*> AAIManager::GetMediumEnemies(int number)
{
	TArray<AMyAI*> array;

	if (number > passivePoolForMedium.Num())
		return array;

	for (int i = 0; i < number; i++)
	{
		AMyAI* enemy = passivePoolForMedium[0];
		passivePoolForMedium.Remove(enemy);
		activePool.Add(enemy);
		array.Add(enemy);
	}

	return array;
}

TArray<AMyAI*> AAIManager::GetLargeEnemies(int number)
{
	TArray<AMyAI*> array;

	if (number > passivePoolForLarge.Num())
		return array;

	for (int i = 0; i < number; i++)
	{
		AMyAI* enemy = passivePoolForLarge[0];
		passivePoolForLarge.Remove(enemy);
		activePool.Add(enemy);
		array.Add(enemy);
	}

	return array;
}

void AAIManager::DeadEnemyManagement(AActor* enemy)
{
	AMyAI* ai = Cast<AMyAI>(enemy);
	ai->isActive = false;
	ai->SetActorLocation(FVector(0, 0, -1000));

	if (ai->type == 2)
		passivePoolForMedium.Add(ai);
	else if (ai->type == 3)
		passivePoolForLarge.Add(ai);
	else
		passivePoolForBasic.Add(ai);
	
	activePool.Remove(ai);
}

void AAIManager::DesactivateAllActiveEnemies()
{
	if (!alreadyDesactivated)
	{
		while (activePool.Num() > 0)
			activePool[0]->Kill(true);

		alreadyDesactivated = true;
		FTimerHandle _;
		GetWorldTimerManager().SetTimer(_, this, &AAIManager::CanDesactivate, 0.5, false);
	}
}

void AAIManager::CanDesactivate()
{
	alreadyDesactivated = false;
}