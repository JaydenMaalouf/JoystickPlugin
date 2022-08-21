#pragma once

#include "JoystickInputDeviceAxisProperties.generated.h"

USTRUCT()
struct JOYSTICKPLUGIN_API FJoystickInputDeviceAxisProperties
{
	GENERATED_BODY()

	FJoystickInputDeviceAxisProperties()
		: bEnabled(true)
		  , InputOffset(0.f)
		  , bInvertInput(false)
		  , InputRangeMin(0.f)
		  , InputRangeMax(1.f)
		  , OutputRangeMin(0.f)
		  , OutputRangeMax(1.f)
		  , bInvertOutput(false)
	{
	}

	/** Whether the axis is enabled. */
	UPROPERTY(EditAnywhere, Category="Axis Properties")
	bool bEnabled;

	/** The amount to offset the axis value by to get the desired range (note that this offset is applied after the value has been inverted). */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="bEnabled"))
	float InputOffset;

	/** Whether the value of the axis as supplied from the driver should be inverted. */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="bEnabled"))
	bool bInvertInput;

	/** The amount to offset the axis value by to get the desired range (note that this offset is applied after the value has been inverted). */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="bEnabled"))
	float InputRangeMin;

	/** The amount to offset the axis value by to get the desired range (note that this offset is applied after the value has been inverted). */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="bEnabled"))
	float InputRangeMax;

	/** The amount to offset the axis value by to get the desired range (note that this offset is applied after the value has been inverted). */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="bEnabled"))
	float OutputRangeMin;

	/** The amount to offset the axis value by to get the desired range (note that this offset is applied after the value has been inverted). */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="bEnabled"))
	float OutputRangeMax;

	/** Whether the value of the axis as supplied from the driver should be inverted. */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="bEnabled"))
	bool bInvertOutput;
};
