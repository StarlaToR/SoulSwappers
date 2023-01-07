#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "ColorStruct.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "PlayerAnimInstance.h"
#include "MyPlayer.generated.h"	


//Delegate signature
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIsDeadSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, int, Current);

UCLASS()
class MAGIC_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float baseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float baseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* springArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* followCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Life, meta = (AllowPrivateAccess = "true"))
		int32 life;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Collider, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* wallRunBox;

	UPROPERTY()
		UUserWidget* pauseWidget;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* bodyMesh;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> pauseMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Life, meta = (AllowPrivateAccess = "true"))
		USkeletalMesh* bluePlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Life, meta = (AllowPrivateAccess = "true"))
		USkeletalMesh* redPlayer;
	UPROPERTY()
	UPlayerAnimInstance *animInstance;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> gameoverMenu;

	UPROPERTY()
		UUserWidget* gameoverWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> healthbar;

	UPROPERTY()
		UUserWidget* healthbarWidget;

	UFUNCTION()
		void OnOverlapWall(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
		void OnEndOverlapWall(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	bool isAlive;
	bool isSliding;
	bool canWallRun;
	bool isReviving;
	bool wantWallRun;
	bool canBeDamaged = true;

	float time;
	float savedTime;
	float pauseTime;
	float timeToMoveAgain;

	int32 maxLife;
	FVector spawnPos;
	FVector recordVelocity;
	FColorStruct color;

	// Delegate signature
	void IsDead();
	void OnHealthChanged(int Current);

	void SetInvicibilityFalse();

public:
	AMyPlayer();

	UPROPERTY()
		FIsDeadSignature isDead;

	UPROPERTY(BlueprintAssignable)
		FOnHealthChangedSignature OnHealthChangedDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool inMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool hasWin = false;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void MenuSound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void DeathSound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void RestartChrono();

	void Swap();
	void Menu();
	void Death();
	void Damage();
	void TakeHeart();
	void RegisterVelocity();
	void SetCheckpoint(FVector checkpoint);
	void UpdatePlayerSpeed(float deltaTime);

	virtual void Tick(float deltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

	// Getter
	bool GetIsAlive() const { return isAlive; }
	int32 GetLife() const { return life; }
	FVector GetSpawnPos() const { return spawnPos; }
	USpringArmComponent* GetSpringArm() const { return springArm; }
	UCameraComponent* GetFollowCamera() const { return followCamera; }

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void JumpSound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void LevelStartSound();

	UMaterialInstanceDynamic* materialDynamic= nullptr;

	void WantJump();
	void DoWallRun();
	void EndWallRun();
	void WantWallRun();
	void CreateDynamicMat();
	void MoveRight(const float value);

	virtual void BeginPlay() override;
};