#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Bullet.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GearsManager.h"
#include "ColorStruct.h"
#include "ColorManager.h"
#include "RocketBullet.h"
#include "PlayerSpirit.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChangedSignature, float, currentAmmo, float, maxAmmo);

UCLASS()
class MAGIC_API APlayerSpirit : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = Multiplayer, meta = (AllowPrivateAccess = "true"))
		bool invert;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AIM, meta = (AllowPrivateAccess = "true"))
		float speedCursorX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AIM, meta = (AllowPrivateAccess = "true"))
		float speedCursorY;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AIM, meta = (AllowPrivateAccess = "true"))
		float depth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MESH, meta = (AllowPrivateAccess = "true"))
		UStaticMesh* meshRed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MESH, meta = (AllowPrivateAccess = "true"))
		UStaticMesh* meshBlue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AIM, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ABullet> bulletBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AIM, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ARocketBullet> rocketBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX", meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* laserFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX", meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* impactLaserFX;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* bodyMesh;

	bool isShooting;
	bool invertMovement;
	bool triggerShootReleased;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool blockInput;

	float PosZ;
	float shootCD;
	float PosZEnd;
	float PosZInit;
	float speedLerp;
	float lerpMovement;
	float maxSpeedCursorX;
	float maxSpeedCursorY;

	APawn* playerPawn;
	APawn* spiritPawn;

	APlayerController* playerController0;
	APlayerController* playerController1;

	FColorStruct color;
	FVector2D cursorPosition;
	AGearsManager* gearsManager;

	void ResetCursor();
	void ChangeMeshColor(bool isRed);

public:
	APlayerSpirit();

	UPROPERTY()
		AColorManager* colorManager;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SwapSound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void LaserSound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ChargeSound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void LaunchSound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void GatlingSound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ShotgunSound();

	FOnAmmoChangedSignature onAmmoChanged;

	void Swap();
	void Menu();
	void EndShoot();
	void StartShoot();
	void MoveCursorUp(float value);
	void SetIsShooting(bool shooting);
	void MoveCursorRight(float value);
	void SetSpeedCursor(float speedX, float speedY);
	void GetSpeedCursor(float& speedX, float& speedY);
	void GetMaxSpeedCursor(float& speedX, float& speedY);
	void SetBlockInput(bool block) { blockInput = block; }

	float GetDepth() { return depth; }
	ColorState GetColor() { return color.colorState; }
	TSubclassOf<ABullet> GetBulletBP() { return bulletBP; }
	TSubclassOf<ARocketBullet> GetRocketBulletBP() { return rocketBP; }
	FVector2D GetCursorPosition() { return cursorPosition; }

	virtual void Tick(float deltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

protected:
	virtual void BeginPlay() override;
};