#pragma once
#include "CoreMinimal.h"
#include "ColorStruct.generated.h"
	 
	 
UENUM()
enum ColorState
{
	Neutral     UMETA(DisplayName = "Neutral"),
	Blue        UMETA(DisplayName = "Blue"),
	Red         UMETA(DisplayName = "Red"),
};

USTRUCT()
struct FColorStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		TEnumAsByte<ColorState> colorState = ColorState::Neutral;

	void ChangeColor()
	{
		if (colorState == ColorState::Blue)
			colorState = ColorState::Red;
		else
			colorState = ColorState::Blue;
	}

	bool IsSameColor(FColorStruct* obj) { return colorState == obj->colorState; }
};