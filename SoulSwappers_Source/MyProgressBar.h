#pragma once
#include "CoreMinimal.h"
#include "PlayerSpirit.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "MyProgressBar.generated.h"


UCLASS()
class MAGIC_API UMyProgressBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void UpdateBar(float Value, float MaxValue);

	void Bind(APlayerSpirit* spirit);

protected:
	UPROPERTY(meta = (BindWidget))
		UProgressBar* MyProgressBar;
};