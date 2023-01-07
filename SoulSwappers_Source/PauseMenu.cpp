#include "PauseMenu.h"


bool UPauseMenu::Initialize()
{
	UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());

	resumeButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("Resume Button"));
	resumeButton->OnClicked.AddDynamic(this, &UPauseMenu::Resume);
	RootWidget->AddChild(resumeButton);

	menuButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("Resume Button"));
	menuButton->OnClicked.AddDynamic(this, &UPauseMenu::ExitToMenu);
	RootWidget->AddChild(menuButton);

	return true;
}

void UPauseMenu::Resume()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UPauseMenu::ExitToMenu() {}