#pragma once

#include "Containers/Array.h"
#include "Data/Input/AxisData.h"
#include "Data/Input/BallData.h"
#include "Data/Input/ButtonData.h"
#include "Data/Input/HatData.h"

#include "JoystickDeviceData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickDeviceData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickState)
		TArray<FAxisData> Axes;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickState)
		TArray<FButtonData> Buttons;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickState)
		TArray<FHatData> Hats;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickState)
		TArray<FBallData> Balls;
};