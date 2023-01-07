#include "PlayerAnimInstance.h"


void UPlayerAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    OwningPawn = TryGetPawnOwner();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float deltaTimeX)
{
    Super::NativeUpdateAnimation(deltaTimeX);

    if (!OwningPawn)
        return;
}