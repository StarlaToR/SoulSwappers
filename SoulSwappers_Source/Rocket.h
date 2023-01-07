#pragma once
#include "CoreMinimal.h"
#include "Weapon.h"
#include "MyAI.h"


class MAGIC_API Rocket : public Weapon
{
public:
	Rocket();
	~Rocket();

	float currentCdShoot;
	float spawnBulletOffSet;

	void EndShoot(APlayerController* spiritController) override;
	void StartShoot(APlayerController* spiritController) override;
	void SpawnBullet(APlayerController* spiritController) override;
	void Tick(float deltaTime, APlayerController* spiritController) override;
	void Shoot(APlayerController* spiritController, float DeltaTime) override;
	AMyAI* FindDirectionBullet(APlayerController* spiritController, FVector& direction);
};