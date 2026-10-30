// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once
#include "Data/Settings/JoystickAxisRemapping.h"

#include "JoystickInputDeviceAxisProperties.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickInputDeviceAxisProperties : public FJoystickAxisRemapping
{
	GENERATED_BODY()

	FJoystickInputDeviceAxisProperties()
		: AxisIndex(INDEX_NONE)
		  , OverrideDisplayName(false)
	{
	}

	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(UIMin="0", ClampMin="0"))
	int AxisIndex;

	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(ConfigRestartRequired=true))
	bool OverrideDisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="OverrideDisplayName", EditConditionHides, ConfigRestartRequired=true))
	FString DisplayName;

	void UpdateProperties(const FJoystickInputDeviceAxisProperties& AxisProperties)
	{
		OverrideDisplayName = AxisProperties.OverrideDisplayName;
		DisplayName = AxisProperties.DisplayName;
		CopyRemapping(AxisProperties);
	}

	void MergeProperties(const FJoystickInputDeviceAxisProperties& AxisProperties)
	{
		if (AxisProperties.OverrideDisplayName)
		{
			OverrideDisplayName = true;
			DisplayName = AxisProperties.DisplayName;
		}

		CopyRemapping(AxisProperties);
	}
};
