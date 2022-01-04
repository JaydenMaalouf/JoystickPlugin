#pragma once

#include "BallData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FBallData
{	
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
		FVector2D Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
		FVector2D PreviousDirection;

	FName KeyName;
};
