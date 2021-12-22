#pragma once

#include "BallData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FBallData
{	
	GENERATED_BODY()
	
	FVector2D Direction;
	FVector2D PreviousDirection;

	FName KeyName;
};
