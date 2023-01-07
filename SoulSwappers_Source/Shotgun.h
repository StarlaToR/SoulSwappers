#pragma once
#include "CoreMinimal.h"
#include "Weapon.h"


class MAGIC_API Shotgun : public Weapon
{
public:
	Shotgun();
	~Shotgun();

	float curCdShoot;
	float speedBullet;
	float spawnBulletOffSet;

	void EndShoot(APlayerController* spiritController) override;
	void StartShoot(APlayerController* spiritController) override;
	void SpawnBullet(APlayerController* spiritController) override;
	void Tick(float deltaTime, APlayerController* spiritController) override;
	void Shoot(APlayerController* spiritController, float DeltaTime) override;

	FVector FindDirectionBullet(APlayerController* spiritController);
};