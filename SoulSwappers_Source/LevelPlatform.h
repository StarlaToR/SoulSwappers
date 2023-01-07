#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColorStruct.h"
#include "Components/BoxComponent.h"
#include "ColorManager.h"
#include "LevelPlatform.generated.h"


UCLASS()
class MAGIC_API ALevelPlatform : public AActor
{
	GENERATED_BODY()

	void ChangePlaformColor();
	void ChangePlatformVisual();

public:
	ALevelPlatform();

	UPROPERTY(EditAnywhere,Category = ColorObject)
		FColorStruct color;

	UFUNCTION()
		void SetPlatformActive(bool state);

	void ChangeSetupPhysics(TEnumAsByte<ColorState> state);

	virtual void Tick(float deltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly,Category = ColorObject)
		float opacityPlatform;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* platformMesh;

	UPROPERTY(EditAnywhere, Category = ColorObject)
		AColorManager* colorManager;

	UPROPERTY(EditAnywhere, Category = ColorObject)
		UMaterial* opaqueMat;

	UPROPERTY(EditAnywhere, Category = ColorObject)
		UMaterial* translucentMat;

	bool isActive;

	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
#endif
};