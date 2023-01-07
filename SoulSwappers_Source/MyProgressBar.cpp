#include "MyProgressBar.h"


void UMyProgressBar::Bind(APlayerSpirit* spirit)
{
	spirit->onAmmoChanged.AddUniqueDynamic(this, &UMyProgressBar::UpdateBar);
}

void UMyProgressBar::UpdateBar(float Value, float MaxValue)
{
	MyProgressBar->SetPercent((Value / MaxValue));
}