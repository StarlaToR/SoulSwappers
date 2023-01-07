#include "WinZone.h"
#include "MyPlayer.h"


AWinZone::AWinZone()
{
	PrimaryActorTick.bCanEverTick = true;

	hitbox = CreateDefaultSubobject<UBoxComponent>(FName("HitBox"));
}

void AWinZone::BeginPlay()
{
	Super::BeginPlay();
	
	hitbox->OnComponentBeginOverlap.AddDynamic(this, &AWinZone::BeginOverlap);
	winWidget = CreateWidget(GetWorld()->GetFirstPlayerController(), winMenu);
}

// Called every frame
void AWinZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWinZone::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		winWidget->AddToViewport();
		AMyPlayer* player = Cast<AMyPlayer>(OtherActor);
		player->inMenu = true;
		player->hasWin = true;
	}
}
