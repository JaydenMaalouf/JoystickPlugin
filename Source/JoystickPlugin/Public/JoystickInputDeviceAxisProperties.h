#pragma once

#include "JoystickInputDeviceAxisProperties.generated.h"

USTRUCT()
struct JOYSTICKPLUGIN_API FJoystickInputDeviceAxisProperties
{
	GENERATED_BODY()

	FJoystickInputDeviceAxisProperties()
		: bEnabled(true)
		, bInverted(false)
		, bGamepadStick(false)
		, RangeMin(0.f)
		, RangeMax(1.f)
		, Offset(0.f)
	{
	}

	/** Whether the axis is enabled. */
	UPROPERTY(EditAnywhere, Category="Axis Properties")
		bool bEnabled;

	/** Whether the value of the axis as supplied from the driver should be inverted. */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="bEnabled"))
		bool bInverted;

	/** Whether this is a gamepad stick, which means it should scale from -1 to 1 instead of 0 to 1 */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="bEnabled"))
		bool bGamepadStick;
	
	/** The amount to offset the axis value by to get the desired range (note that this offset is applied after the value has been inverted). */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="bEnabled"))
		float RangeMin;
	
	/** The amount to offset the axis value by to get the desired range (note that this offset is applied after the value has been inverted). */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="bEnabled"))
		float RangeMax;

	/** The amount to offset the axis value by to get the desired range (note that this offset is applied after the value has been inverted). */
	UPROPERTY(EditAnywhere, Category="Axis Properties", meta=(EditCondition="bEnabled"))
		float Offset;
};