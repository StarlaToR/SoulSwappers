#include "MyAI.h"
#include "GearsManager.h"


AMyAI::AMyAI()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(TEXT("Enemy"));

	isActive = false;
	targetPlayer = true;

	hp = 5.f;
	type = 1.f;
	speed = 1000.f;
	timeBeforeImpact = 5.f;

	collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision component"));
	collision->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	collision->OnComponentBeginOverlap.AddDynamic(this, &AMyAI::BeginOverlap);

	autoAimCollider = CreateDefaultSubobject<USphereComponent>(TEXT("AutoAim component"));
	autoAimCollider->AttachToComponent(collision, FAttachmentTransformRules::KeepRelativeTransform);

	sphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere mesh"));
	sphereMesh->AttachToComponent(collision, FAttachmentTransformRules::KeepRelativeTransform);
	sphereMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void AMyAI::BeginPlay()
{
	Super::BeginPlay();
}

void AMyAI::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	AMyPlayer* player = Cast<AMyPlayer>(target1);
	if (player == nullptr)
		player = Cast<AMyPlayer>(target2);

	if (isActive && !player->inMenu)
	{
		FVector playerPos = player->GetActorLocation();
		FVector currentPos = GetActorLocation();

		if (playerPos.X > currentPos.X + 500)
		{
			Kill(true);
			return;
		}

		FVector dir = playerPos - currentPos, normalizedDir = dir;
		normalizedDir.Normalize();
		float playerSpeed = player->GetCharacterMovement()->MaxWalkSpeed;

		if (dir.Size() > (speed + playerSpeed) * timeBeforeImpact && targetPlayer)
		{
			direction = normalizedDir;
		}
		else if (targetPlayer)
		{
			direction = (playerPos + FVector::ForwardVector * timeBeforeImpact * playerSpeed - currentPos) / (speed * timeBeforeImpact);

			direction.Normalize();
			targetPlayer = false;
		}

		SetActorLocation(currentPos + direction * speed * deltaTime);
	}
}

void AMyAI::SetUp(FVector spawn, ColorState col)
{
	spawnPos = spawn;
	color.colorState = col;
	SetEnemyColor(color.colorState);
}

void AMyAI::Activate()
{
	SetActorLocation(spawnPos);
	isActive = true;
	targetPlayer = true;

	if (target1 == nullptr)
		target1 = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (target2 == nullptr)
		target2 = UGameplayStatics::GetPlayerCharacter(GetWorld(), 1);
}

void AMyAI::KillSound_Implementation() {}
void AMyAI::Kill(bool isSystem)
{
	if (!isSystem)
	{
		UNiagaraComponent* comp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), enemyVFXDeath, GetActorLocation());
		if (color.colorState == ColorState::Blue)
		{
			comp->SetVariableLinearColor("Color1", FLinearColor::Blue);
			comp->SetVariableLinearColor("SparkColor", FLinearColor::Blue);
		}
		KillSound();

	}
	pool->DeadEnemyManagement(this);
}

void AMyAI::RocketSound_Implementation() {}
void AMyAI::Damage(int damage)
{
	hp -= damage;

	TArray<AActor*> gearManager;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGearsManager::StaticClass(), gearManager);
	AGearsManager* gear = Cast<AGearsManager>(gearManager[0]);
	if (gear->weaponType == WeaponType::ROCKET)
		RocketSound();

	if (hp <= 0)
		Kill(false);
}

void AMyAI::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AMyPlayer>(OtherActor) && OtherActor->ActorHasTag("Player"))
	{
		AMyPlayer* player = Cast<AMyPlayer>(OtherActor);

		if (player)
		{
			player->Damage();
		}

		Kill(true);
	}
}

void AMyAI::SetEnemyColor(ColorState col)
{
	if (col == ColorState::Blue)
		sphereMesh->SetStaticMesh(meshBlue);
	else
		sphereMesh->SetStaticMesh(meshRed);
}