// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "JoystickInputDeviceButtonProperties.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickInputDeviceButtonProperties
{
	GENERATED_BODY()

	FJoystickInputDeviceButtonProperties()
		: ButtonIndex(-1),
		  OverrideDisplayName(false),
		  InvertOutput(false)
	{
	}

	UPROPERTY(EditAnywhere, Category="Button Properties", meta=(UIMin="0", ClampMin="0"))
	int ButtonIndex;

	UPROPERTY(EditAnywhere, Category="Button Properties")
	bool OverrideDisplayName;

	UPROPERTY(EditAnywhere, Category="Button Properties", meta=(EditCondition="OverrideDisplayName", EditConditionHides))
	FString DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Button Properties")
	bool InvertOutput;

	void UpdateProperties(const FJoystickInputDeviceButtonProperties& ButtonProperties)
	{
		OverrideDisplayName = ButtonProperties.OverrideDisplayName;
		DisplayName = ButtonProperties.DisplayName;
		InvertOutput = ButtonProperties.InvertOutput;
	}
};
