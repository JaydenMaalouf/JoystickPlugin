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
		  , RemappingEnabled(true)
		  , InputOffset(0.f)
		  , InvertInput(false)
		  , InputRangeMin(0.f)
		  , InputRangeMax(1.f)
		  , OutputRangeMin(0.f)
		  , OutputRangeMax(1.f)
		  , InvertOutput(false)
	{
	}

	/** The index of the Axis to apply remapping to. */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(UIMin="0", ClampMin="0"))
	int AxisIndex;

	/** Whether the axis remapping is enabled. */
	UPROPERTY(EditAnywhere, Category="Axis Properties")
	bool RemappingEnabled;

	/** The amount to offset the axis value by to get the desired range (note that this offset is applied after the value has been inverted). */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled"))
	float InputOffset;

	/** Whether the value of the axis as supplied from the driver should be inverted. */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled"))
	bool InvertInput;

	/** The amount to offset the axis value by to get the desired range (note that this offset is applied after the value has been inverted). */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled"))
	float InputRangeMin;

	/** The amount to offset the axis value by to get the desired range (note that this offset is applied after the value has been inverted). */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled"))
	float InputRangeMax;

	/** The amount to offset the axis value by to get the desired range (note that this offset is applied after the value has been inverted). */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled"))
	float OutputRangeMin;

	/** The amount to offset the axis value by to get the desired range (note that this offset is applied after the value has been inverted). */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled"))
	float OutputRangeMax;

	/** Whether the value of the axis as supplied from the driver should be inverted. */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled"))
	bool InvertOutput;
};
