#include "LevelPlatform.h"


ALevelPlatform::ALevelPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	isActive = true;

	opacityPlatform = 0.5f;

	platformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubePlatform"));
	platformMesh->SetupAttachment(RootComponent);
	platformMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
}

void ALevelPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (color.colorState == ColorState::Red)
	{
		platformMesh->SetCollisionProfileName(TEXT("RedObjectPreset"));
		colorManager->redStateDelegate.AddDynamic(this, &ALevelPlatform::SetPlatformActive);
	}

	if (color.colorState == ColorState::Blue)
	{
		platformMesh->SetCollisionProfileName(TEXT("BlueObjectPreset"));
		colorManager->blueStateDelegate.AddDynamic(this, &ALevelPlatform::SetPlatformActive);
		SetPlatformActive(false);
	}
}

void ALevelPlatform::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void ALevelPlatform::SetPlatformActive(bool state)
{
	isActive = state;
	ChangePlatformVisual();
}

#pragma region Visual
void ALevelPlatform::ChangePlatformVisual()
{
	if (!isActive)
		platformMesh->SetMaterial(0, translucentMat);
	else
		platformMesh->SetMaterial(0, opaqueMat);
}

#pragma endregion

#if WITH_EDITOR
	void ALevelPlatform::ChangeSetupPhysics(TEnumAsByte<ColorState> state)
	{

		switch (state)
		{
		case ColorState::Blue:
			platformMesh->SetCollisionProfileName(TEXT("BlueObjectPreset"));
			break;
		case ColorState::Red:
			platformMesh->SetCollisionProfileName(TEXT("RedObjectPreset"));
			break;
		default:
			break;
		}
	}

	void ALevelPlatform::PostEditChangeProperty(struct FPropertyChangedEvent& e)
	{
		Super::PostEditChangeProperty(e);
		static const FName NAME_ColorStruct = FName(TEXT("colorState"));

		if (e.Property)
			if (e.Property->GetFName() == NAME_ColorStruct)
				ChangeSetupPhysics(color.colorState);
	}
#endif