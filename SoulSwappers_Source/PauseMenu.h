#pragma once
#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "PauseMenu.generated.h"


UCLASS()
class MAGIC_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

	UPROPERTY()
		UButton* menuButton;

	UPROPERTY()
		UButton* resumeButton;

	UFUNCTION()
		void Resume();

	UFUNCTION()
		void ExitToMenu();
};