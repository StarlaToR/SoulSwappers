#pragma once
#include "CoreMinimal.h"
#include "AIPattern.h"
#include "FunnelPattern.generated.h"


UCLASS()
class MAGIC_API AFunnelPattern : public AAIPattern
{
	GENERATED_BODY()
public:
	void Behavior();
	void MonoColorBehaviour();

	void Execute() override;
};
