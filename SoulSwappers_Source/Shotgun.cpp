#include "Shotgun.h"
#include "PlayerSpirit.h"
#include "GearsManager.h"


Shotgun::Shotgun()
{
	cooldownShoot = 0.25f;
	speedBullet = 20000.f;
	spawnBulletOffSet = 150.f;
	curCdShoot = 0.f;
}

Shotgun::~Shotgun() {}

void Shotgun::StartShoot(APlayerController* spiritController)
{
	APlayerSpirit* spirit = Cast<APlayerSpirit>(spiritController->GetPawn());
	spirit->SetIsShooting(true);

	if (curCdShoot <= 0)
	{
		curCdShoot = cooldownShoot;
		SpawnBullet(spiritController);
	}
}

void Shotgun::Shoot(APlayerController* spiritController, float DeltaTime)
{
	if (curCdShoot <= 0)
	{
		curCdShoot = cooldownShoot;
		SpawnBullet(spiritController);
	}
}

FVector Shotgun::FindDirectionBullet(APlayerController* spiritController)
{
	APlayerSpirit* spirit = Cast<APlayerSpirit>(spiritController->GetPawn());

	FVector worldPositionBulletSpawn, worldDirectionBullet, DirBullet;

	APlayerController* playerController = Cast<APlayerController>(spirit->GetWorld()->GetFirstPlayerController());
	playerController->DeprojectScreenPositionToWorld(spirit->GetCursorPosition().X, spirit->GetCursorPosition().Y, worldPositionBulletSpawn, worldDirectionBullet);

	//Raycast
	FHitResult OutHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(spirit->GetOwner());
	FVector startRayCast = worldPositionBulletSpawn + worldDirectionBullet * 100;
	FVector endRayCast = startRayCast + worldDirectionBullet * 20000;

	bool isHit = spirit->GetWorld()->LineTraceSingleByChannel(OutHit, startRayCast, endRayCast, ECC_Visibility, Params);

	if (isHit)
	{
		DirBullet = (OutHit.GetActor()->GetTransform().GetLocation() - spirit->GetTransform().GetLocation()).GetSafeNormal();
	}
	else
	{
		FVector TargetLocation = worldPositionBulletSpawn + worldDirectionBullet * spirit->GetDepth();
		DirBullet = (TargetLocation - spirit->GetTransform().GetLocation()).GetSafeNormal();
	}

	return DirBullet;
}


void Shotgun::SpawnBullet(APlayerController* spiritController)
{
	APlayerSpirit* spirit = Cast<APlayerSpirit>(spiritController->GetPawn());

	FVector DirBullet = FindDirectionBullet(spiritController);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = spiritController->GetPawn();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	//TO UPDATE//
	ABullet* bullet = spirit->GetWorld()->SpawnActor<ABullet>(spirit->GetBulletBP(), spirit->GetTransform().GetLocation() + DirBullet.GetSafeNormal() * spawnBulletOffSet, FRotator(0, 0, 0), SpawnParams);
	bullet->SetColor(spirit->GetColor());
	bullet->SetDirection(DirBullet);
	bullet->SetSpeed(speedBullet);
	bullet->ActiveCollision();

	for (int i = 0; i < 5; i++)
	{
		FVector newDirection1 = DirBullet;
		FVector newDirection2 = DirBullet;

		if (i == 1)
		{
			newDirection1 = DirBullet.RotateAngleAxis(2.0f, DirBullet.UpVector).RotateAngleAxis(1.45f, DirBullet.RightVector);
			newDirection2 = DirBullet.RotateAngleAxis(2.0f, DirBullet.UpVector);
		}
		else if (i == 2)
		{
			newDirection1 = DirBullet.RotateAngleAxis(-2.0f, DirBullet.UpVector).RotateAngleAxis(1.45f, DirBullet.RightVector);
			newDirection2 = DirBullet.RotateAngleAxis(-2.0f, DirBullet.UpVector);
		}
		else if (i == 3)
		{
			newDirection1 = DirBullet.RotateAngleAxis(2.0f, DirBullet.UpVector).RotateAngleAxis(-1.45f, DirBullet.RightVector);
			newDirection2 = DirBullet.RotateAngleAxis(2.0f, DirBullet.RightVector);
		}
		else if (i == 4)
		{
			newDirection1 = DirBullet.RotateAngleAxis(-2.0f, DirBullet.UpVector).RotateAngleAxis(-1.45f, DirBullet.RightVector);
			newDirection2 = DirBullet.RotateAngleAxis(-2.0f, DirBullet.RightVector);
		}

		ABullet* bullet1 = spirit->GetWorld()->SpawnActor<ABullet>(spirit->GetBulletBP(), spirit->GetTransform().GetLocation() + newDirection1.GetSafeNormal() * spawnBulletOffSet, FRotator(0, 0, 0), SpawnParams);
		bullet1->SetColor(spirit->GetColor());
		bullet1->SetDirection(newDirection1);
		bullet1->SetSpeed(speedBullet);
		bullet1->ActiveCollision();

		ABullet* bullet2 = spirit->GetWorld()->SpawnActor<ABullet>(spirit->GetBulletBP(), spirit->GetTransform().GetLocation() + newDirection2.GetSafeNormal() * spawnBulletOffSet, FRotator(0, 0, 0), SpawnParams);
		bullet2->SetColor(spirit->GetColor());
		bullet2->SetDirection(newDirection2);
		bullet2->SetSpeed(speedBullet);
		bullet2->ActiveCollision();
	}
}

void Shotgun::EndShoot(APlayerController* spiritController)
{
	APlayerSpirit* spirit = Cast<APlayerSpirit>(spiritController->GetPawn());
	spirit->SetIsShooting(false);
}

void Shotgun::Tick(float deltaTime, APlayerController* spiritController)
{
	if (curCdShoot > 0)
		curCdShoot -= deltaTime;
}