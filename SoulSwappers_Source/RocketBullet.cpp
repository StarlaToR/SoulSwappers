#include "RocketBullet.h"
#include "MyAI.h"
#include "DrawDebugHelpers.h"


ARocketBullet::ARocketBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	sphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	RootComponent = sphereMesh;

	sphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	sphereCollider->SetupAttachment(sphereMesh);
	trailFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));
	trailFX->SetupAttachment(sphereMesh);
}

void ARocketBullet::BeginPlay()
{
	Super::BeginPlay();
	radiusExplosion = 400.f;
	currentTimeLife = timeLife;
}

void ARocketBullet::ActiveCollision()
{
	sphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ARocketBullet::BeginOverlap);
}

void ARocketBullet::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Enemy")))
	{
		AMyAI* AI = Cast<AMyAI>(OtherActor);

		if (AI->GetColor().colorState == color.colorState)
		{
			AI->Damage();
			UNiagaraComponent* comp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), explosionFX, GetActorLocation());
			if (color.colorState == ColorState::Blue)
				comp->SetNiagaraVariableLinearColor("Color2", FLinearColor::Blue);

			TArray<AActor*> enemies;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyAI::StaticClass(), enemies);
			for (int i = 0; i < enemies.Num(); i++)
			{
				AMyAI* enemy = Cast<AMyAI>(enemies[i]);
				if (enemy->isActive)
				{
					float distanceRocketEnemy = (enemy->GetTransform().GetLocation() - GetActorLocation()).Size();
					if (distanceRocketEnemy < radiusExplosion && enemy->GetColor().colorState == color.colorState)
						enemy->Damage(1);
				}
			}

			this->Destroy();
		}
	}
}

void ARocketBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (currentTimeLife >= 0)
		currentTimeLife -= DeltaTime;
	else
		Destroy();

	if (aiToFollow != nullptr && aiToFollow->isActive)
	{
		FVector dir = (aiToFollow->GetTransform().GetLocation() - GetActorLocation()).GetSafeNormal();
		AddActorWorldOffset(dir * speed * DeltaTime);
	}
	else
	{
		AddActorWorldOffset(direction * speed * DeltaTime);

		if (currentTimeLife > timeLife - searchTargetDuration)
			FindValidTarget();
	}
}

void ARocketBullet::FindValidTarget()
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

void ARocketBullet::SetDirection(FVector dir)
{
	direction = dir;
}

void ARocketBullet::SetSpeed(float newSpeed)
{
	speed = newSpeed;
}

void ARocketBullet::SetTimeLife(float newTimeLife)
{
	currentTimeLife = newTimeLife;
	timeLife = newTimeLife;
}

void ARocketBullet::SetColor(ColorState newColor)
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
			realMat->SetVectorParameterValue("ColorBullet", FLinearColor::Red);
			trailFX->SetNiagaraVariableLinearColor("Color1", redColor);
		}
		else
		{
			realMat->SetVectorParameterValue("ColorBullet", FLinearColor::Blue);
			trailFX->SetNiagaraVariableLinearColor("Color1", blueColor);
		}
	}
}

void ARocketBullet::IncreaseSize()
{
	sphereMesh->SetRelativeScale3D(sphereMesh->GetRelativeScale3D() * 2);
	sphereCollider->SetSphereRadius(sphereCollider->GetScaledSphereRadius() * 2);
}

void ARocketBullet::SetPawnToFollow(AMyAI* pawn)
{
	aiToFollow = pawn;
}