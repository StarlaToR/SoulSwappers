// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "HealthBar.generated.h"


class AMyPlayer;

UCLASS()
class MAGIC_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* heart1;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* heart2;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* heart3;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* heart4;

    UPROPERTY(meta = (BindWidget))
        UImage* heart5;

    UFUNCTION()
        void UpdateHealth(int current);

    TArray<UImage*> hearts;

    void Bind(AMyPlayer* character);

    virtual void NativeConstruct() override;
};
