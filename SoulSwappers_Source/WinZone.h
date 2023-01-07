// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "ToolBox.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "WinZone.generated.h"

UCLASS()
class MAGIC_API AWinZone : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
		UBoxComponent* hitbox;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> winMenu;

	UPROPERTY()
		UUserWidget* winWidget;

public:	
	AWinZone();

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;
};
