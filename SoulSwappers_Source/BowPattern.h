#pragma once
#include "CoreMinimal.h"
#include "AIPattern.h"
#include "BowPattern.generated.h"


UCLASS()
class MAGIC_API ABowPattern : public AAIPattern
{
	GENERATED_BODY()
public:
	void Execute() override;

	void Behavior();

	void MonoColorBehaviour();
};
