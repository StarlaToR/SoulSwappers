#include "Gatling.h"
#include "ToolBox.h"
#include "ColorStruct.h"
#include "PlayerSpirit.h"


Gatling::Gatling()
{
	cooldownShoot = 0.07f;
	currentCdShoot = 0.f;
	spawnBulletOffSet = 200.f;
}

Gatling::~Gatling() {}

void Gatling::StartShoot(APlayerController* spiritController)
{
	APlayerSpirit* spirit = Cast<APlayerSpirit>(spiritController->GetPawn());
	spirit->SetIsShooting(true);

	if (currentCdShoot <= 0)
	{
		currentCdShoot = cooldownShoot;
		SpawnBullet(spiritController);
	}
}

void Gatling::Shoot(APlayerController* spiritController, float deltaTime)
{
	if (currentCdShoot <= 0)
	{
		currentCdShoot = cooldownShoot;
		SpawnBullet(spiritController);
	}
}

AMyAI* Gatling::FindDirectionBullet(APlayerController* spiritController, FVector& direction)
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

	AMyAI* actor = Cast<AMyAI>(OutHit.GetActor());
	if (isHit && actor != nullptr) //AutoAim
	{
		direction = (OutHit.GetActor()->GetTransform().GetLocation() - spirit->GetTransform().GetLocation()).GetSafeNormal();
	}
	else
	{
		FVector TargetLocation = worldPositionBulletSpawn + worldDirectionBullet * spirit->GetDepth();
		direction = (TargetLocation - spirit->GetTransform().GetLocation()).GetSafeNormal();
	}

	return actor;
}

void Gatling::SpawnBullet(APlayerController* spiritController)
{
	FVector DirBullet;
	AMyAI* pawnToFollow = FindDirectionBullet(spiritController, DirBullet);
	APlayerSpirit* spirit = Cast<APlayerSpirit>(spiritController->GetPawn());

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = spiritController->GetPawn();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//Spawn Bullet
	ABullet* bullet = spirit->GetWorld()->SpawnActor<ABullet>(spirit->GetBulletBP(), spirit->GetTransform().GetLocation() + DirBullet * spawnBulletOffSet, FRotator(0, 0, 0), SpawnParams);
	bullet->SetDirection(DirBullet);
	bullet->SetPawnToFollow(pawnToFollow);
	bullet->SetColor(spirit->GetColor());
	bullet->ActiveCollision();
}

void Gatling::EndShoot(APlayerController* spiritController)
{
	APlayerSpirit* spirit = Cast<APlayerSpirit>(spiritController->GetPawn());
	spirit->SetIsShooting(false);
}

void Gatling::Tick(float deltaTime, APlayerController* spiritController)
{
	if (currentCdShoot > 0)
		currentCdShoot -= deltaTime;
}