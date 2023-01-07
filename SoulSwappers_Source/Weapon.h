#pragma once
#include "CoreMinimal.h"
#include "Engine/World.h"


class MAGIC_API Weapon
{
public:
	Weapon();
	virtual ~Weapon();

	float cooldownShoot;

	void ResetCooldownShoot();

	virtual void EndShoot(APlayerController* spiritController);
	virtual void StartShoot(APlayerController* spiritController);
	virtual void SpawnBullet(APlayerController* spiritController);
	virtual void Tick(float deltaTime, APlayerController* spiritController);
	virtual void Shoot(APlayerController* spiritController, float DeltaTime);
};