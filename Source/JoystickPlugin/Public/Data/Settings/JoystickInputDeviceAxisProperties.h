// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "JoystickInputDeviceAxisProperties.generated.h"

USTRUCT()
struct JOYSTICKPLUGIN_API FJoystickInputDeviceAxisProperties
{
	GENERATED_BODY()

	FJoystickInputDeviceAxisProperties()
		: AxisIndex(-1)
		  , OverrideDisplayName(false)
		  , RemappingEnabled(true)
		  , InputOffset(0.0f)
		  , InvertInput(false)
		  , RerangeInput(false)
		  , InputRangeMin(0.0f)
		  , InputRangeMax(1.0f)
		  , RerangeOutput(false)
		  , OutputRangeMin(0.0f)
		  , OutputRangeMax(1.0f)
		  , InvertOutput(false)
	{
	}

	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(UIMin="0", ClampMin="0"))
	int AxisIndex;

	UPROPERTY(EditAnywhere, Category="Axis Properties")
	bool OverrideDisplayName;

	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="OverrideDisplayName", EditConditionHides))
	FString DisplayName;

	UPROPERTY(EditAnywhere, Category="Axis Properties")
	bool RemappingEnabled;

	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled", EditConditionHides))
	float InputOffset;

	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled", EditConditionHides))
	bool InvertInput;

	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled", EditConditionHides))
	bool RerangeInput;

	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled && RerangeInput", EditConditionHides))
	float InputRangeMin;

	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled && RerangeInput", EditConditionHides))
	float InputRangeMax;

	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled", EditConditionHides))
	bool RerangeOutput;

	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled && RerangeOutput", EditConditionHides))
	float OutputRangeMin;

	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled && RerangeOutput", EditConditionHides))
	float OutputRangeMax;

	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled", EditConditionHides))
	bool InvertOutput;
};
