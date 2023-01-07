#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "MyCursorWidget.generated.h"


UCLASS()
class MAGIC_API UMyCursorWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UImage* imageCursor;

public:
	UImage* GetImageCursor() { return imageCursor; }
};