#include "MysteryBox.h"
#include "MyPlayer.h"
#include "GearsManager.h"
#include "MyHUD.h"


AMysteryBox::AMysteryBox()
{
	PrimaryActorTick.bCanEverTick = true;

	angle = 0.f;

	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Colllider"));
	boxCollider->SetupAttachment(RootComponent);
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AMysteryBox::BeginOverlap);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubePlatform"));
	mesh->SetupAttachment(boxCollider);
}

void AMysteryBox::BeginPlay()
{
	Super::BeginPlay();
}

void AMysteryBox::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	angle += deltaTime * speedRotation;
	if (angle > 360.f)
		angle -= 360.f;

	FRotator rot = FRotator(0, angle, 0);
	SetActorRotation(rot);
}

void AMysteryBox::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AMyPlayer>(OtherActor) != nullptr)
	{
		TArray<AActor*> gearsManagers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGearsManager::StaticClass(), gearsManagers);
		AGearsManager* gearManager = Cast<AGearsManager>(gearsManagers[0]);
		gearManager->SetRandomWeapon(true);

		AMyHUD* HUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if (gearManager->weaponType == WeaponType::LASER)
			HUD->SetVisibilityChargeUI(ESlateVisibility::Visible);
		else
			HUD->SetVisibilityChargeUI(ESlateVisibility::Hidden);

		this->Destroy();
	}
}