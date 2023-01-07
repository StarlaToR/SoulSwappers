#include "PlayerSpirit.h"
#include "DrawDebugHelpers.h"
#include "ToolBox.h"
#include "MyProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayer.h"
#include "MyHUD.h"
#include "MyPlayer.h"


APlayerSpirit::APlayerSpirit()
{
	PrimaryActorTick.bCanEverTick = true;
	triggerShootReleased = true;
	isShooting = false;
	invert = false;

	shootCD = 0;

	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	bodyMesh->SetupAttachment(RootComponent);

	laserFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Laser"));
	laserFX->SetupAttachment(bodyMesh);

	impactLaserFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ImpactLaser"));
	impactLaserFX->SetupAttachment(bodyMesh);
}

void APlayerSpirit::BeginPlay()
{
	Super::BeginPlay();

	invertMovement = false;

	PosZEnd = 10.f;
	PosZ = PosZInit;
	PosZInit = -10.f;
	speedLerp = 1.0f;
	lerpMovement = 0.f;
	maxSpeedCursorX = speedCursorX;
	maxSpeedCursorY = speedCursorY;

	//HUD Setup
	AMyHUD* HUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (HUD)
	{
		HUD->InitializeHUD();
		cursorPosition = HUD->GetCursorPosition();
		auto AmmoBar = Cast<UMyProgressBar>(HUD->GetAmmoBar());

		if (AmmoBar)
			AmmoBar->Bind(this);
	}

	//Gears Setup
	TArray<AActor*> gears;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGearsManager::StaticClass(), gears);
	gearsManager = Cast<AGearsManager>(gears[0]);
	gearsManager->SetRandomWeapon();
	gearsManager->laser->SetVFX(laserFX, impactLaserFX);
	laserFX->SetVariableLinearColor("Color1", FLinearColor::Blue);
	impactLaserFX->SetVariableLinearColor("Color1", FLinearColor::Blue);
	laserFX->Deactivate();
	impactLaserFX->Deactivate();

	if (gearsManager->weaponType == WeaponType::LASER)
	{
		HUD->SetVisibilityChargeUI(ESlateVisibility::Visible);
	}
	else
	{
		HUD->SetVisibilityChargeUI(ESlateVisibility::Hidden);
		laserFX->Deactivate();
		impactLaserFX->Deactivate();
	}

	// --------- To upgrade -----------
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AColorManager::StaticClass(), FoundActors);
	if (Cast<AColorManager>(FoundActors[0]))
		colorManager = Cast<AColorManager>(FoundActors[0]);
	//-------------------------------

	//Controller Setup
	playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	spiritPawn = GetController()->GetPawn();
	playerController0 = GetWorld()->GetFirstPlayerController();
	playerController1 = Cast<APlayerController>(this->GetController());

	color.colorState = ColorState::Blue;
	bodyMesh->SetStaticMesh(meshBlue);
}

void APlayerSpirit::LaunchSound_Implementation() {}
void APlayerSpirit::GatlingSound_Implementation() {}
void APlayerSpirit::ShotgunSound_Implementation() {}
void APlayerSpirit::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	shootCD -= deltaTime;
	if (!triggerShootReleased && shootCD <= 0.f)
	{
		if (gearsManager->weaponType == WeaponType::ROCKET)
			LaunchSound();
		else if (gearsManager->weaponType == WeaponType::SHOTGUN)
			ShotgunSound();
		else if (gearsManager->weaponType == WeaponType::GATLING)
			GatlingSound();

		shootCD = gearsManager->GetWeapon()->cooldownShoot;
	}

	SetActorLocation(playerPawn->GetTransform().GetLocation() + FVector(0, 150, 150 + PosZ));

	gearsManager->GetWeapon()->Tick(deltaTime, Cast<APlayerController>(Controller));

	if (isShooting && !triggerShootReleased)
		gearsManager->GetWeapon()->Shoot(Cast<APlayerController>(Controller), deltaTime);


	PosZ = FMath::Lerp(PosZInit, PosZEnd, lerpMovement);

	if (!invertMovement)
	{
		if (lerpMovement >= 0.8f || lerpMovement <= 0.2f)
			lerpMovement += deltaTime * (speedLerp * 0.75f);
		else
			lerpMovement += deltaTime * speedLerp;
	}
	else
	{
		if (lerpMovement >= 0.8f || lerpMovement <= 0.2f)
			lerpMovement -= deltaTime * (speedLerp * 0.75f);
		else
			lerpMovement -= deltaTime * speedLerp;
	}

	if (lerpMovement > 1.f)
	{
		invertMovement = true;
		lerpMovement = 1.0f;
	}
	else if (lerpMovement < 0.f)
	{
		invertMovement = false;
		lerpMovement = 0.f;
	}
}

void APlayerSpirit::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	check(playerInputComponent);
	playerInputComponent->BindAction("Menu", IE_Pressed, this, &APlayerSpirit::Menu);
	playerInputComponent->BindAction("Swap", IE_Pressed, this, &APlayerSpirit::Swap);
	playerInputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerSpirit::StartShoot);
	playerInputComponent->BindAction("Shoot", IE_Released, this, &APlayerSpirit::EndShoot);
	playerInputComponent->BindAxis("CursorRight", this, &APlayerSpirit::MoveCursorRight);
	playerInputComponent->BindAxis("CursorUp", this, &APlayerSpirit::MoveCursorUp);
}

void APlayerSpirit::MoveCursorUp(float value)
{
	if (Controller != nullptr)
	{
		AMyHUD* HUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		cursorPosition += FVector2D(0, -1) * value * speedCursorY * GetWorld()->GetDeltaSeconds();

		if (HUD)
		{
			HUD->SetCursorPosition(cursorPosition);

			if (cursorPosition.Y < 0)
				cursorPosition.Y = 0;
			else if (cursorPosition.Y > HUD->GetViewportSize().Y)
				cursorPosition.Y = HUD->GetViewportSize().Y;
		}
	}
}

void APlayerSpirit::MoveCursorRight(float value)
{
	if (Controller != nullptr)
	{
		AMyHUD* HUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		cursorPosition += FVector2D(1, 0) * value * speedCursorX * GetWorld()->GetDeltaSeconds();

		if (HUD)
		{
			HUD->SetCursorPosition(cursorPosition);

			if (cursorPosition.X < 0)
				cursorPosition.X = 0;
			else if (cursorPosition.X > HUD->GetViewportSize().X)
				cursorPosition.X = HUD->GetViewportSize().X;
		}
	}
}

void APlayerSpirit::Menu()
{
	AMyPlayer* player = Cast<AMyPlayer>(playerPawn);
	player->Menu();
}

void APlayerSpirit::ResetCursor()
{
	AMyHUD* HUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUD->ResetCursorPosition();
}

void APlayerSpirit::ChargeSound_Implementation() {}
void APlayerSpirit::StartShoot()
{
	if (shootCD <= 0.f && !blockInput)
	{
		if (gearsManager->weaponType == WeaponType::GATLING)
			GatlingSound();
		else if (gearsManager->weaponType == WeaponType::ROCKET)
			LaunchSound();
		else if (gearsManager->weaponType == WeaponType::SHOTGUN)
			ShotgunSound();
		else
			ChargeSound();

		shootCD = gearsManager->GetWeapon()->cooldownShoot;
		gearsManager->GetWeapon()->StartShoot(Cast<APlayerController>(Controller));
	}

	triggerShootReleased = false;
}

void APlayerSpirit::LaserSound_Implementation() {}
void APlayerSpirit::EndShoot()
{
	triggerShootReleased = true;
	gearsManager->GetWeapon()->EndShoot(Cast<APlayerController>(Controller));

	if (gearsManager->weaponType == WeaponType::LASER)
	{
		Laser* laser = (Laser*)gearsManager->GetWeapon();
		if (laser->timeLeftUsingLaser > 0.f)
			LaserSound();
	}
}

void APlayerSpirit::SwapSound_Implementation() {}
void APlayerSpirit::Swap()
{

	if (!blockInput)
	{
		AMyPlayer* player = Cast<AMyPlayer>(playerPawn);
		player->RegisterVelocity();
		ResetCursor();
		EndShoot();

		if (!invert)
		{
			playerController1->Possess(playerPawn);
			playerController0->Possess(spiritPawn);
			playerController0->SetViewTarget(playerPawn);
			ChangeMeshColor(true);

			laserFX->SetVariableLinearColor("Color1", FLinearColor::Red);
			impactLaserFX->SetVariableLinearColor("Color1", FLinearColor::Red);

			if (colorManager)
				colorManager->ChanceColorActive(false);

			invert = true;
		}
		else
		{
			playerController0->Possess(playerPawn);
			playerController1->Possess(spiritPawn);
			ChangeMeshColor(false);

			laserFX->SetVariableLinearColor("Color1", FLinearColor::Blue);
			impactLaserFX->SetVariableLinearColor("Color1", FLinearColor::Blue);

			if (colorManager)
				colorManager->ChanceColorActive(true);

			invert = false;
		}

		SwapSound();
		player->Swap();
		color.ChangeColor();
	}

}

void APlayerSpirit::ChangeMeshColor(bool isRed)
{
	if (isRed)
		bodyMesh->SetStaticMesh(meshRed);
	else
		bodyMesh->SetStaticMesh(meshBlue);
}

void APlayerSpirit::SetIsShooting(bool shooting)
{
	isShooting = shooting;
}

void APlayerSpirit::SetSpeedCursor(float speedX, float speedY)
{
	speedCursorX = speedX;
	speedCursorY = speedY;
}

void APlayerSpirit::GetSpeedCursor(float& speedX, float& speedY)
{
	speedX = speedCursorX;
	speedY = speedCursorY;
}

void APlayerSpirit::GetMaxSpeedCursor(float& speedX, float& speedY)
{
	speedX = maxSpeedCursorX;
	speedY = maxSpeedCursorY;
}