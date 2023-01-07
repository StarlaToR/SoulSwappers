#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColorManager.generated.h"


// Delegate signature
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBlueStateSignature, bool, state);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRedStateSignature, bool, red);

UCLASS()
class MAGIC_API AColorManager : public AActor
{
	GENERATED_BODY()

public:
	AColorManager();

	UPROPERTY()
		FRedStateSignature redStateDelegate;

	UPROPERTY()
		FBlueStateSignature blueStateDelegate;

	void ChanceColorActive(bool isRed);

	virtual void Tick(float deltaTime) override;

protected:
	virtual void BeginPlay() override;

	// Function signature
	void BlueState(bool state);
	void RedState(bool state);
};