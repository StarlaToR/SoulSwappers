#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyCursorWidget.h"
#include "MyProgressBar.h"
#include "MyHUD.generated.h"


UCLASS()
class MAGIC_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMyHUD();

	void InitializeHUD();
	void ResetCursorPosition();
	void SetShouldDrawHUD(bool draw);
	void SetCursorPosition(FVector2D position);
	void SetVisibilityChargeUI(ESlateVisibility visibility);

	virtual void DrawHUD() override;

	// Getter
	FVector2D GetCursorPosition() { return cursorPosition; };
	FVector2D GetViewportSize() { return viewportSize; };
	UMyProgressBar* GetAmmoBar() { return ammoBar; };

protected:
	UPROPERTY(meta = (AllowPrivateAccess = "true"))
		UMyProgressBar* ammoBar;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
		UMyCursorWidget* aimCursor;

	UPROPERTY(EditAnywhere, Category = "ClassType")
		TSubclassOf<UMyCursorWidget> widgetAimCursor;

	UPROPERTY(EditAnywhere, Category = "ClassType")
		TSubclassOf<UMyProgressBar> widgetAmmoBar;

	bool shouldDrawHUD;

	FVector2D viewportSize;
	FVector2D cursorPosition;
};