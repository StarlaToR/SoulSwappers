#include "Laser.h"
#include "DrawDebugHelpers.h"
#include "PlayerSpirit.h"	


Laser::Laser()
{
	usingLaser = false;

	radius = 150.f;
	timeCharge = 0.f;
	chargeNeeded = .4f;
	cooldownDamage = 0.2f;
	timeUsingLaser = 0.8f;
	curCooldownDamage = 0.f;
	timeLeftUsingLaser = 0.f;
}

Laser::~Laser() {}

void Laser::StartShoot(APlayerController* spiritController)
{
	if (timeLeftUsingLaser <= 0)
	{
		APlayerSpirit* spirit = Cast<APlayerSpirit>(spiritController->GetPawn());
		spirit->SetIsShooting(true);

		usingLaser = false;

		timeCharge = 0.f;
		timeLeftUsingLaser = 0.f;

		float maxSpeedX, maxSpeedY;
		spirit->GetMaxSpeedCursor(maxSpeedX, maxSpeedY);
		spirit->SetSpeedCursor(maxSpeedX, maxSpeedY);
	}
}
void Laser::SetVFX(UNiagaraComponent* laser, UNiagaraComponent* impact)
{
	laserFX = laser;
	impactFX = impact;
}

void Laser::Shoot(APlayerController* spiritController, float deltaTime)
{
	if (timeLeftUsingLaser <= 0)
	{
		APlayerSpirit* spirit = Cast<APlayerSpirit>(spiritController->GetPawn());

		if (timeCharge < chargeNeeded && !usingLaser)
		{
			timeCharge += deltaTime;
			spirit->onAmmoChanged.Broadcast(timeCharge, chargeNeeded);
		}
		else if (timeCharge >= chargeNeeded && !usingLaser)
		{
			timeLeftUsingLaser = timeUsingLaser;
		}
	}
}

FVector Laser::FindDirectionBullet(APlayerController* spiritController)
{
	APlayerSpirit* spirit = Cast<APlayerSpirit>(spiritController->GetPawn());

	FVector worldPositionBulletSpawn, worldDirectionBullet, dirBullet;

	APlayerController* playerController = Cast<APlayerController>(spirit->GetWorld()->GetFirstPlayerController());
	playerController->DeprojectScreenPositionToWorld(spirit->GetCursorPosition().X, spirit->GetCursorPosition().Y, worldPositionBulletSpawn, worldDirectionBullet);

	//Raycast
	FHitResult outHit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(spirit->GetOwner());
	FVector startRayCast = worldPositionBulletSpawn + worldDirectionBullet * 100;
	FVector endRayCast = startRayCast + worldDirectionBullet * 20000;

	bool isHit = spirit->GetWorld()->LineTraceSingleByChannel(outHit, startRayCast, endRayCast, ECC_Visibility, params);

	if (isHit) //AutoAim
	{
		laserFX->SetNiagaraVariableVec3("LaserEnd", outHit.ImpactPoint);
		impactFX->SetWorldLocation(outHit.ImpactPoint);
		dirBullet = (outHit.ImpactPoint - spirit->GetTransform().GetLocation()).GetSafeNormal();
	}
	else
	{
		FVector targetLocation = worldPositionBulletSpawn + worldDirectionBullet * spirit->GetDepth();
		laserFX->SetNiagaraVariableVec3("LaserEnd", targetLocation);
		impactFX->SetWorldLocation(targetLocation);
		dirBullet = (targetLocation - spirit->GetTransform().GetLocation()).GetSafeNormal();
	}

	return dirBullet;
}

void Laser::SpawnBullet(APlayerController* spiritController)
{
	APlayerSpirit* spirit = Cast<APlayerSpirit>(spiritController->GetPawn());

	FVector worldPositionBulletSpawn, worldDirectionBullet;

	APlayerController* playerController = Cast<APlayerController>(spirit->GetWorld()->GetFirstPlayerController());
	playerController->DeprojectScreenPositionToWorld(spirit->GetCursorPosition().X, spirit->GetCursorPosition().Y, worldPositionBulletSpawn, worldDirectionBullet);

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = spiritController->GetPawn();
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	direction = FindDirectionBullet(spiritController);
}

void Laser::EndShoot(APlayerController* spiritController)
{
	usingLaser = true;
	timeCharge = 0.f;

	APlayerSpirit* spirit = Cast<APlayerSpirit>(spiritController->GetPawn());
	spirit->onAmmoChanged.Broadcast(timeCharge, chargeNeeded);

	laserFX->Activate();
	impactFX->Activate();

	if (timeLeftUsingLaser > 0.f)
	{
		float speedX, speedY;
		spirit->GetSpeedCursor(speedX, speedY);
		spirit->SetSpeedCursor(speedX * 0.75f, speedY * 0.75f);
	}
}

void Laser::Tick(float deltaTime, APlayerController* spiritController)
{
	APlayerSpirit* spirit = Cast<APlayerSpirit>(spiritController->GetPawn());

	if (usingLaser && timeLeftUsingLaser > 0)
	{
		SpawnBullet(spiritController);
		timeLeftUsingLaser -= deltaTime;
		spirit->onAmmoChanged.Broadcast(timeLeftUsingLaser, timeUsingLaser);

		if (curCooldownDamage <= 0)
		{
			TArray<AActor*> enemies;
			UGameplayStatics::GetAllActorsOfClass(spiritController->GetWorld(), AMyAI::StaticClass(), enemies);

			for (int i = 0; i < enemies.Num(); i++)
			{
				//Find if the laser hits enemies
				AMyAI* enemy = Cast<AMyAI>(enemies[i]);
				if (enemy->isActive)
				{
					float distanceLaserEnemy = FMath::PointDistToLine(enemy->GetTransform().GetLocation(), direction, spirit->GetTransform().GetLocation());
					if (distanceLaserEnemy < radius && spirit->GetColor() == enemy->GetColor().colorState)
						enemy->Damage(1);
				}
			}

			curCooldownDamage = cooldownDamage;
		}
		else
		{
			curCooldownDamage -= deltaTime;
		}
	}
	else if (usingLaser && timeLeftUsingLaser <= 0)
	{
		StopLaser(spirit);
	}
}

void Laser::StopLaser(APlayerSpirit* spirit)
{
	usingLaser = false;
	timeCharge = 0.f;
	curCooldownDamage = 0.f;
	spirit->SetIsShooting(false);

	laserFX->DeactivateImmediate();
	impactFX->DeactivateImmediate();

	float maxSpeedX, maxSpeedY;
	spirit->GetMaxSpeedCursor(maxSpeedX, maxSpeedY);
	spirit->SetSpeedCursor(maxSpeedX, maxSpeedY);
}