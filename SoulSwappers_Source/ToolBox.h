#pragma once
#include "CoreMinimal.h"


#if WITH_EDITOR
#define Debug(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(TEXT(x), __VA_ARGS__));}
#define DebugError(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(x), __VA_ARGS__));}
#define DebugWarning(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(x), __VA_ARGS__));}
#else
#define Debug(x, ...)
#define DebugError(x, ...) 
#define DebugWarning(x, ...)
#endif

class MAGIC_API ToolBox
{
public:
	ToolBox();
	~ToolBox();
};