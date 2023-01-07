#include "Bumper.h"
#include "GameFramework/Character.h"
#include "ToolBox.h"


ABumper::ABumper()
{
	PrimaryActorTick.bCanEverTick = true;

	BumpStrengh = 500.f;

	direction = FVector(1.f, 0.f, 0.f);

	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Colllider"));
	boxCollider->SetupAttachment(RootComponent);
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &ABumper::BeginOverlap);
	boxCollider->SetBoxExtent(FVector(50, 50, 50));

	UStaticMesh* cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubePlatform"));
	mesh->SetStaticMesh(cubeMesh);
	mesh->SetupAttachment(boxCollider);

	arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction Arrow"));
	arrow->SetupAttachment(boxCollider);
	arrow->SetRelativeLocation(FVector(0, 0, 0));
}

void ABumper::BumperSound_Implementation() {}
void ABumper::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(PlayerTag))
	{
		BumperSound();
		BumperTarget(OtherActor);
	}
}

void ABumper::BumperTarget(AActor* target)
{
	ACharacter* charaTarget = Cast<ACharacter>(target);
	FVector dir = direction;
	dir.Normalize();
	charaTarget->LaunchCharacter(dir * BumpStrengh, true, true);
}

#if WITH_EDITOR
	void ABumper::PostEditChangeProperty(struct FPropertyChangedEvent& e) 
	{
		Super::PostEditChangeProperty(e);
	
		if (e.Property)
			ChangeArrowOrientation(e);
	}

	void ABumper::ChangeArrowOrientation(struct FPropertyChangedEvent& e)
	{
		static const FName NAME_ColorStruct = FName(TEXT("direction"));
		if (e.Property->GetFName() == TEXT("X") || e.Property->GetFName() == TEXT("Y") || e.Property->GetFName() == TEXT("Z") || e.Property->GetFName() == NAME_ColorStruct)
		{
			FVector dir = direction;
			dir.Normalize();
			FQuat rot = dir.Rotation().Quaternion();
			arrow->SetRelativeRotation(rot);
		}
	}
#endif