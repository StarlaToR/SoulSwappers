#include "Rocket.h"
#include "ToolBox.h"
#include "ColorStruct.h"
#include "PlayerSpirit.h"


Rocket::Rocket()
{
	cooldownShoot = 0.5f;
	spawnBulletOffSet = 200.f;
	currentCdShoot = 0.f;
}

Rocket::~Rocket() {}

void Rocket::StartShoot(APlayerController* spiritController)
{
	APlayerSpirit* spirit = Cast<APlayerSpirit>(spiritController->GetPawn());
	spirit->SetIsShooting(true);

	if (currentCdShoot <= 0)
	{
		currentCdShoot = cooldownShoot;
		SpawnBullet(spiritController);
	}
}

void Rocket::Shoot(APlayerController* spiritController, float DeltaTime)
{
	if (currentCdShoot <= 0)
	{
		currentCdShoot = cooldownShoot;
		SpawnBullet(spiritController);
	}
}

AMyAI* Rocket::FindDirectionBullet(APlayerController* spiritController, FVector& direction)
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

void Rocket::SpawnBullet(APlayerController* spiritController)
{
	APlayerSpirit* spirit = Cast<APlayerSpirit>(spiritController->GetPawn());
	FVector DirBullet;
	AMyAI* pawnToFollow = FindDirectionBullet(spiritController, DirBullet);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = spiritController->GetPawn();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//Spawn Bullet
	ARocketBullet* bullet = spirit->GetWorld()->SpawnActor<ARocketBullet>(spirit->GetRocketBulletBP(), spirit->GetTransform().GetLocation() + DirBullet * spawnBulletOffSet, FRotator(0, 0, 0), SpawnParams);
	bullet->SetDirection(DirBullet);
	DirBullet.Normalize();
	bullet->SetActorRotation(DirBullet.Rotation());
	bullet->SetPawnToFollow(pawnToFollow);
	bullet->SetSpeed(7000);
	bullet->SetColor(spirit->GetColor());
	bullet->ActiveCollision();
}

void Rocket::EndShoot(APlayerController* spiritController)
{
	APlayerSpirit* spirit = Cast<APlayerSpirit>(spiritController->GetPawn());
	spirit->SetIsShooting(false);
}


void Rocket::Tick(float deltaTime, APlayerController* spiritController)
{
	if (currentCdShoot > 0)
		currentCdShoot -= deltaTime;
}