// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "JoystickInputDeviceAxisProperties.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickInputDeviceAxisProperties
{
	GENERATED_BODY()

	FJoystickInputDeviceAxisProperties()
		: AxisIndex(-1)
		  , OverrideDisplayName(false)
		  , RemappingEnabled(false)
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(ConfigRestartRequired=true))
	bool OverrideDisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="OverrideDisplayName", EditConditionHides, ConfigRestartRequired=true))
	FString DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties")
	bool RemappingEnabled;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled", EditConditionHides))
	float InputOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled", EditConditionHides))
	bool InvertInput;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled", EditConditionHides))
	bool RerangeInput;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled && RerangeInput", EditConditionHides))
	float InputRangeMin;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled && RerangeInput", EditConditionHides))
	float InputRangeMax;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled", EditConditionHides))
	bool RerangeOutput;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled && RerangeOutput", EditConditionHides))
	float OutputRangeMin;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled && RerangeOutput", EditConditionHides))
	float OutputRangeMax;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled", EditConditionHides))
	bool InvertOutput;

	void UpdateProperties(const FJoystickInputDeviceAxisProperties& AxisProperties)
	{
		OverrideDisplayName = AxisProperties.OverrideDisplayName;
		DisplayName = AxisProperties.DisplayName;
		RemappingEnabled = AxisProperties.RemappingEnabled;
		InputOffset = AxisProperties.InputOffset;
		InvertInput = AxisProperties.InvertInput;
		RerangeInput = AxisProperties.RerangeInput;
		InputRangeMin = AxisProperties.InputRangeMin;
		InputRangeMax = AxisProperties.InputRangeMax;
		RerangeOutput = AxisProperties.RerangeOutput;
		OutputRangeMin = AxisProperties.OutputRangeMin;
		OutputRangeMax = AxisProperties.OutputRangeMax;
		InvertOutput = AxisProperties.InvertOutput;
	}
};
