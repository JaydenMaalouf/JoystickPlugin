// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "JoystickSensorInformation.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickSensorInformation
{
	GENERATED_BODY()

	FJoystickSensorInformation()
		: Enabled(false)
		  , Supported(false)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Sensor Config")
	bool Enabled;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Sensor Config")
	bool Supported;
};
