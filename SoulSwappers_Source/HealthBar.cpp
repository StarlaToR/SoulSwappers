#include "HealthBar.h"
#include "MyPlayer.h"


void UHealthBar::Bind(AMyPlayer* character)
{
    character->OnHealthChangedDelegate.AddUniqueDynamic(this, &UHealthBar::UpdateHealth);
}

void UHealthBar::NativeConstruct()
{
    hearts.Add(heart1);
    hearts.Add(heart2);
    hearts.Add(heart3);
    hearts.Add(heart4);
    hearts.Add(heart5);
}

void UHealthBar::UpdateHealth(int Current)
{
    for (int i = 0; i < 5; i++)
    {
        if (i < Current)
            hearts[4 - i]->SetVisibility(ESlateVisibility::Visible);
        else
            hearts[4 - i]->SetVisibility(ESlateVisibility::Hidden);
    }
}