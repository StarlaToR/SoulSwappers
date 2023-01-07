#include "MyPlayer.h"
#include "ToolBox.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "PlayerAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "HealthBar.h"
#include "LevelPlatform.h"



AMyPlayer::AMyPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("Player");

	inMenu = false;
	isAlive = true;
	isSliding = false;
	canWallRun = false;

	life = 5;
	maxLife = life;

	time = 0.f;
	savedTime = 0.f;
	baseTurnRate = 45.f;
	baseLookUpRate = 45.f;
	timeToMoveAgain = 0.f;

	GetCapsuleComponent()->InitCapsuleSize(38.f, 88.f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 800.0f, 0.0f);
	GetCharacterMovement()->FallingLateralFriction = 3.f;
	GetCharacterMovement()->MaxAcceleration = 30000.f;
	GetCharacterMovement()->JumpZVelocity = 2000.f;
	GetCharacterMovement()->MaxWalkSpeed = 1500.f;
	GetCharacterMovement()->GravityScale = 6.f;
	GetCharacterMovement()->AirControl = 1.f;

	wallRunBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WallRunBox"));
	wallRunBox->SetupAttachment(RootComponent);
	wallRunBox->AddLocalOffset(FVector(0.f, 0.f, -20.f));
	wallRunBox->SetRelativeScale3D(FVector(1.f, 1.5f, 1.25f));

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(RootComponent);
	springArm->TargetArmLength = 700.0f; // The camera follows at this distance behind the character
	springArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	followCamera->SetupAttachment(springArm, USpringArmComponent::SocketName); // Attach the camera to the end of the spring arm
	followCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	bodyMesh->SetupAttachment(RootComponent);
}

void AMyPlayer::LevelStartSound_Implementation() {}
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	wallRunBox->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::OnOverlapWall);
	wallRunBox->OnComponentEndOverlap.AddDynamic(this, &AMyPlayer::OnEndOverlapWall);

	TSubclassOf<APlayerStart> classToFind = APlayerStart::StaticClass();
	TArray<AActor*> playerStartActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, playerStartActors);

	APlayerStart* playerStart = Cast<APlayerStart>(playerStartActors[0]);

	SetActorLocation(playerStart->GetTransform().GetLocation());
	color.colorState = ColorState::Red;

	USkeletalMeshComponent* mesh = GetMesh();
	UAnimInstance* anim = mesh->GetAnimInstance();
	animInstance = Cast<UPlayerAnimInstance>(anim);

	LevelStartSound();
	CreateDynamicMat();

	pauseWidget = CreateWidget(GetWorld()->GetFirstPlayerController(), pauseMenu);

	gameoverWidget = CreateWidget(GetWorld()->GetFirstPlayerController(), gameoverMenu);
	healthbarWidget = CreateWidget(GetWorld()->GetFirstPlayerController(), healthbar);
	healthbarWidget->AddToViewport();
	UHealthBar* hb = Cast<UHealthBar>(healthbarWidget);

	if (hb != nullptr)
		hb->Bind(this);
}

void AMyPlayer::CreateDynamicMat()
{
	UMaterialInterface* mat = bodyMesh->GetMaterial(0);
	materialDynamic = UMaterialInstanceDynamic::Create(mat, this);
	materialDynamic->SetVectorParameterValue(TEXT("Tint"), FLinearColor::Red);
	bodyMesh->SetMaterial(0, materialDynamic);
}

void AMyPlayer::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (isReviving)
		if (GetWorld()->TimeSeconds >= timeToMoveAgain)
			isReviving = false;

	if (canWallRun && wantWallRun && !isSliding)
		DoWallRun();

	if (isSliding && !canWallRun)
		EndWallRun();
		
	if (GetCharacterMovement()->MaxWalkSpeed <= 8000 && !inMenu && GetCharacterMovement()->Velocity.X > 1)
		UpdatePlayerSpeed(deltaTime);

	if (Controller && !inMenu && isAlive && !isReviving)
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(direction, 1);
	}

	if (CanJump() && animInstance->IsJumping)
		animInstance->IsJumping = false;
}

void AMyPlayer::UpdatePlayerSpeed(float deltaTime)
{
	time += deltaTime;
	GetCharacterMovement()->MaxWalkSpeed = 5.1663E-14f * powf(time, 6)
										 + 2.0819E-10f * powf(time, 5)
										 - 3.29E-07f * powf(time, 4)
										 + 0.0003f * powf(time, 3)
										 - 0.106f * powf(time, 2)
										 + 25.8674f * time
										 + 1700; // Starting speed
}

void AMyPlayer::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	check(playerInputComponent);
	playerInputComponent->BindAction("WallRun", IE_Pressed, this, &AMyPlayer::WantWallRun);
	playerInputComponent->BindAction("WallRun", IE_Released, this, &AMyPlayer::EndWallRun);
	playerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyPlayer::WantJump);
	playerInputComponent->BindAction("Menu", IE_Pressed, this, &AMyPlayer::Menu);
	playerInputComponent->BindAxis("MoveRight", this, &AMyPlayer::MoveRight);
}

void AMyPlayer::SetCheckpoint(FVector checkpoint)
{
	spawnPos = checkpoint;
	savedTime = time;
}

void AMyPlayer::MoveRight(const float value)
{
	if (Controller != nullptr && value != 0.0f && isAlive && !inMenu && !isReviving)
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(direction, value * 0.8f);
	}
}

void AMyPlayer::MenuSound_Implementation() {}
void AMyPlayer::Menu()
{
	MenuSound();
	if (!inMenu && !hasWin && isAlive)
	{
		MenuSound();
		if (inMenu == false)
		{
			inMenu = true;
			pauseWidget->AddToViewport();
			pauseTime = time;
		}
		else
		{
			inMenu = false;
			pauseWidget->RemoveFromViewport();
			time = pauseTime;
		}
	}
}

void AMyPlayer::RegisterVelocity()
{
	recordVelocity = GetCharacterMovement()->Velocity;
}

void AMyPlayer::Swap()
{
	if (!inMenu)
	{
		EndWallRun();
		if (GetCapsuleComponent()->GetCollisionProfileName() == "BlueObjectPreset")
		{
			materialDynamic->SetVectorParameterValue(TEXT("Tint"), FLinearColor::Red);
			bodyMesh->SetMaterial(0, materialDynamic);
			GetCapsuleComponent()->SetCollisionProfileName(TEXT("RedObjectPreset"));
			USkeletalMeshComponent* mesh = GetMesh();

			mesh->SetSkeletalMesh(redPlayer);
		}
		else
		{
			materialDynamic->SetVectorParameterValue(TEXT("Tint"), FLinearColor::Blue);
			bodyMesh->SetMaterial(0, materialDynamic);
			GetCapsuleComponent()->SetCollisionProfileName(TEXT("BlueObjectPreset"));
			USkeletalMeshComponent* mesh = GetMesh();
			mesh->SetSkeletalMesh(bluePlayer);
		}

		GetCharacterMovement()->Velocity = FVector(recordVelocity);
		color.ChangeColor();

		Controller->SetControlRotation(FRotator(0.f, 0.f, 0.f));
	}
}

void AMyPlayer::OnOverlapWall(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag(FName("Wall")) && Cast<ALevelPlatform>(OtherActor))
	{
		ALevelPlatform* wall = Cast<ALevelPlatform>(OtherActor);
		if(wall->color.colorState == color.colorState)
			canWallRun = true;
	}
}

void AMyPlayer::OnEndOverlapWall(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	canWallRun = false;
}

void AMyPlayer::WantWallRun()
{
	wantWallRun = true;
}

void AMyPlayer::DoWallRun()
{
	isSliding = true;
	GetCharacterMovement()->GravityScale = 0.f;
	GetCharacterMovement()->StopMovementImmediately();
}

void AMyPlayer::EndWallRun()
{
	if (isSliding)
	{
		isSliding = false;
		wantWallRun = false;
		GetCharacterMovement()->GravityScale = 6.f;
		GetCharacterMovement()->Launch(FVector(0.f, 0.f, 2000.f));
	}
}

void AMyPlayer::DeathSound_Implementation() {}
void AMyPlayer::Death()
{
	DeathSound();
	time = savedTime;
	isDead.Broadcast();
	SetActorLocation(spawnPos);
	life--;
	OnHealthChangedDelegate.Broadcast(life);

	isReviving = true;
	RestartChrono();
	timeToMoveAgain = GetWorld()->TimeSeconds + 1.5f;

	if (life <= 0)
	{
		isAlive = false;
		gameoverWidget->AddToViewport();
	}
}

void AMyPlayer::SetInvicibilityFalse()
{
	canBeDamaged = true;
}

void AMyPlayer::Damage()
{
	if (canBeDamaged)
	{
		canBeDamaged = false;
		life--;
		OnHealthChangedDelegate.Broadcast(life);
		if (life <= 0)
		{
			Death();
		}
		FTimerHandle _;
		GetWorld()->GetTimerManager().SetTimer(_, this, &AMyPlayer::SetInvicibilityFalse, 0.2, false);
	}
}

void AMyPlayer::JumpSound_Implementation() {}
void AMyPlayer::WantJump()
{
	if (CanJump() && !isReviving)
	{
		Jump();
		JumpSound();
		animInstance->IsJumping = true;
	}
}

void AMyPlayer::TakeHeart()	
{
	if(life < 5)
		life++;

	OnHealthChangedDelegate.Broadcast(life);
}

void AMyPlayer::RestartChrono_Implementation() {}