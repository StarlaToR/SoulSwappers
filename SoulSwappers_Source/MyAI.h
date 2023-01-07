#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ColorStruct.h"
#include "MyPlayer.h"
#include "ToolBox.h"
#include "AIPool.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyAI.generated.h"


UCLASS()
class MAGIC_API AMyAI : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
		bool targetPlayer;

	UPROPERTY(EditAnywhere)
		float hp;

	UPROPERTY(EditAnywhere)
		float speed;

	UPROPERTY(EditAnywhere)
		float timeBeforeImpact;

	UPROPERTY()
		FVector spawnPos;

	UPROPERTY()
		FVector direction;

	UPROPERTY(EditAnywhere)
		USphereComponent* collision;

	UPROPERTY(EditAnywhere)
		USphereComponent* autoAimCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MESH, meta = (AllowPrivateAccess = "true"))
		UStaticMesh* meshBlue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MESH, meta = (AllowPrivateAccess = "true"))
		UStaticMesh* meshRed;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* sphereMesh;

	UPROPERTY()
		FColorStruct color;
	UPROPERTY()
		AActor* target2;
	UPROPERTY()
		AActor* target1;

	void SetEnemyColor(ColorState col);

public:
	AMyAI();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
		class UNiagaraSystem* enemyVFXDeath;

	UPROPERTY()
		bool isActive;

	UPROPERTY(EditAnywhere)
		float type;

	UPROPERTY()
		AAIPool* pool;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void KillSound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void RocketSound();

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Activate();
	void Kill(bool isSystem);
	void Damage(int damage = 1);
	void SetUp(FVector spawn, ColorState col = ColorState::Blue);

	FColorStruct GetColor() { return color; }

	virtual void Tick(float deltaTime) override;

protected:
	virtual void BeginPlay() override;
};