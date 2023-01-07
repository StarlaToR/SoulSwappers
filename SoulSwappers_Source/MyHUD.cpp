#include "MyHUD.h"
#include "GameFramework/PlayerController.h"


AMyHUD::AMyHUD()
{
	PrimaryActorTick.bCanEverTick = true;
	shouldDrawHUD = false;
}

void AMyHUD::InitializeHUD()
{
	if (IsValid(widgetAimCursor))
		aimCursor = Cast<UMyCursorWidget>(CreateWidget(GetWorld(), widgetAimCursor));
	aimCursor->AddToViewport();

	if (IsValid(widgetAmmoBar))
		ammoBar = Cast<UMyProgressBar>(CreateWidget(GetWorld(), widgetAmmoBar));
	ammoBar->AddToViewport();

	viewportSize = FVector2D(1, 1);

	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(viewportSize);

	SetCursorPosition(viewportSize / 2.f);

	shouldDrawHUD = true;
}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();

	if (shouldDrawHUD)
	{
		if (GEngine && GEngine->GameViewport)
			GEngine->GameViewport->GetViewportSize(viewportSize);

		aimCursor->SetPositionInViewport(FVector2D(cursorPosition.X - 8.f, cursorPosition.Y - 8.f), true);
	}
}

void AMyHUD::ResetCursorPosition()
{
	SetCursorPosition(viewportSize / 2.f);
}

void AMyHUD::SetCursorPosition(FVector2D position)
{
	cursorPosition = position;
}

void AMyHUD::SetVisibilityChargeUI(ESlateVisibility visibility)
{
	ammoBar->SetVisibility(visibility);
}