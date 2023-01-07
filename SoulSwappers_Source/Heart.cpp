#include "Heart.h"
#include "MyPlayer.h"


AHeart::AHeart()
{
	PrimaryActorTick.bCanEverTick = true;

	angle = 0.f;

	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Colllider"));
	boxCollider->SetupAttachment(RootComponent);
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AHeart::BeginOverlap);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubePlatform"));
	mesh->SetupAttachment(boxCollider);
}

void AHeart::BeginPlay()
{
	Super::BeginPlay();
}

void AHeart::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	angle += deltaTime * speedRotation;
	if (angle > 360.f)
		angle -= 360.f;

	FRotator rot = FRotator(0, angle, 0);
	SetActorRotation(rot);
}

void AHeart::HeartSound_Implementation() {}
void AHeart::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AMyPlayer>(OtherActor) != nullptr)
	{
		HeartSound();
		AMyPlayer* player = Cast<AMyPlayer>(OtherActor);
		player->TakeHeart();
		this->Destroy();
	}
}