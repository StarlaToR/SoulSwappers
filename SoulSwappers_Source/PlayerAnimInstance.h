#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"


UCLASS(transient, Blueprintable, BlueprintType)
class MAGIC_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
        bool IsJumping;

    APawn* OwningPawn;

    virtual void NativeUpdateAnimation(float deltaTimeX);

    virtual void NativeInitializeAnimation() override;
};