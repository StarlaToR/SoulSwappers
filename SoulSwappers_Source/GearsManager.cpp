#include "GearsManager.h"


AGearsManager::AGearsManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGearsManager::BeginPlay()
{
	Super::BeginPlay();

	beginGame = true;

	weaponType = WeaponType::SHOTGUN;
	laser = new Laser();
	rocket = new Rocket();
	gatling = new Gatling();
	shotgun = new Shotgun();
}

void AGearsManager::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void AGearsManager::SetWeaponSound_Implementation() {}
void AGearsManager::SetWeapon(WeaponType type)
{
	weaponType = type;
	laser->laserFX->DeactivateImmediate();
	laser->impactFX->DeactivateImmediate();
	if (type == WeaponType::GATLING)
		weapon = gatling;
	else if (type == WeaponType::SHOTGUN)
		weapon = shotgun;
	else if (type == WeaponType::LASER)
		weapon = laser;
	else if (type == WeaponType::ROCKET)
		weapon = rocket;

	if (beginGame)
		beginGame = false;
	else
		SetWeaponSound();
}

void AGearsManager::SetRandomWeapon(bool deactiveLaser)
{
	int32 rand = FMath::RandRange(0, 2);

	if (rand == weaponType)
		rand++;

	if (rand == 3)
		rand = 0;
	if (deactiveLaser)
	{
		laser->laserFX->DeactivateImmediate();
		laser->impactFX->DeactivateImmediate();
	}
	if (rand == 0)
	{
		weaponType = WeaponType::GATLING;
		weapon = gatling;
	}
	else if (rand == 1)
	{
		weaponType = WeaponType::ROCKET;
		weapon = rocket;
	}
	else if (rand == 2)
	{
		weaponType = WeaponType::LASER;
		weapon = laser;
	}
}