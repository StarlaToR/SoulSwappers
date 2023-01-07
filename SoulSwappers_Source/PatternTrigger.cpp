#include "PatternTrigger.h"


APatternTrigger::APatternTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	hitbox = CreateDefaultSubobject<UBoxComponent>(FName("HitBox"));
}

void APatternTrigger::BeginPlay()
{
	Super::BeginPlay();
	hitbox->OnComponentBeginOverlap.AddDynamic(this, &APatternTrigger::BeginOverlap);
	hasBeenTriggered = false;
}

void APatternTrigger::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void APatternTrigger::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!hasBeenTriggered && OtherActor->ActorHasTag(TEXT("Player")))
	{
		for (int i = 0; i < patterns.Num(); i++)
			patterns[i]->Execute();

		hasBeenTriggered = true;
		FTimerHandle _;
		GetWorldTimerManager().SetTimer(_, this, &APatternTrigger::triggerAvailable, 0.5, false);
	}
}

void APatternTrigger::triggerAvailable()
{
	hasBeenTriggered = false;
}