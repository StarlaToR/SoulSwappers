#pragma once
#include "CoreMinimal.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Weapon.h"

class APlayerSpirit;

class MAGIC_API Laser : public Weapon
{
public:
	Laser();
	~Laser();

	bool usingLaser;

	float radius;
	float timeCharge;
	float chargeNeeded;
	float timeUsingLaser;
	float cooldownDamage;
	float curCooldownDamage;
	float timeLeftUsingLaser;

	FVector direction;
	UNiagaraComponent* laserFX;
	UNiagaraComponent* impactFX;
	void SetVFX(UNiagaraComponent* laser, UNiagaraComponent* impact);
	void EndShoot(APlayerController* spiritController) override;
	void StartShoot(APlayerController* spiritController) override;
	void SpawnBullet(APlayerController* spiritController) override;
	void Tick(float deltaTime, APlayerController* spiritController) override;
	void Shoot(APlayerController* spiritController, float deltaTime) override;
	void StopLaser(APlayerSpirit* spirit);
	FVector FindDirectionBullet(APlayerController* spiritController);
};