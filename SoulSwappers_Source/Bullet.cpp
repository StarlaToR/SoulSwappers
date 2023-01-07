#include "Bullet.h"
#include "MyAI.h"


ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	searchTargetDuration = 1.5f;

	sphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	RootComponent = sphereMesh;

	sphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	sphereCollider->SetupAttachment(sphereMesh);
	
	trailFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));
	trailFX->SetupAttachment(sphereMesh);
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();

	currentTimeLife = timeLife;
}

void ABullet::ActiveCollision()
{
	sphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ABullet::BeginOverlap);
}

void ABullet::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Enemy")))
	{
		AMyAI* AI = Cast<AMyAI>(OtherActor);

		if(AI->GetColor().colorState == color.colorState)
			AI->Damage();

		this->Destroy();
	}
}

void ABullet::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (currentTimeLife >= 0)
		currentTimeLife -= deltaTime;
	else
		Destroy();

	if (aiToFollow != nullptr && aiToFollow->isActive)
	{
		FVector dir = (aiToFollow->GetTransform().GetLocation() - GetActorLocation()).GetSafeNormal();
		AddActorWorldOffset(dir * speed * deltaTime);
	}
	else
	{
		AddActorWorldOffset(direction * speed * deltaTime);

		if (currentTimeLife > timeLife - searchTargetDuration)
			FindValidTarget();
	}
}

void ABullet::FindValidTarget()
{
	//Raycast
	FHitResult OutHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	FVector startRayCast = GetActorLocation();
	FVector endRayCast = startRayCast + direction * 20000;

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, startRayCast, endRayCast, ECC_Visibility, Params);

	AMyAI* actor = Cast<AMyAI>(OutHit.GetActor());
	if (isHit && actor)
	{
		SetPawnToFollow(actor);
		aiToFollow->isActive;
	}
}

void ABullet::SetDirection(FVector dir)
{
	direction = dir;
}

void ABullet::SetSpeed(float newSpeed)
{
	speed = newSpeed;
}

void ABullet::SetTimeLife(float newTimeLife)
{
	currentTimeLife = newTimeLife;
	timeLife = newTimeLife;
}

void ABullet::SetColor(ColorState newColor)
{
	this->color.colorState = newColor;

	auto mat = sphereMesh->GetMaterial(0);
	if (mat)
	{
		auto realMat = Cast<UMaterialInstanceDynamic>(mat->GetMaterial());
		if (!realMat)
			realMat = sphereMesh->CreateAndSetMaterialInstanceDynamic(0);

		if (color.colorState == ColorState::Red)
		{
			trailFX->SetTemplate(trailRed); 
			realMat->SetVectorParameterValue("ColorBullet", FLinearColor::Red);
		}
		else
			realMat->SetVectorParameterValue("ColorBullet", FLinearColor::Blue);
	}
}

void ABullet::IncreaseSize()
{
	sphereMesh->SetRelativeScale3D(sphereMesh->GetRelativeScale3D() * 2);
	sphereCollider->SetSphereRadius(sphereCollider->GetScaledSphereRadius() * 2);
}

void ABullet::SetPawnToFollow(AMyAI* pawn)
{
	aiToFollow = pawn;
}