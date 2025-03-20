// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "JoystickHapticInformation.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickHapticInformation
{
	GENERATED_BODY()

	FJoystickHapticInformation()
		: Supported(false)
		  , AxesCount(0)
		  , Constant(false)
		  , Sine(false)
		  , Triangle(false)
		  , SawToothUp(false)
		  , SawToothDown(false)
		  , Ramp(false)
		  , Spring(false)
		  , Damper(false)
		  , Inertia(false)
		  , Friction(false)
		  , Custom(false)
		  , Gain(false)
		  , AutoCenter(false)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Haptic Config")
	bool Supported;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Haptic Config")
	int AxesCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Haptic Config/Supported Effects")
	bool Constant;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Haptic Config/Supported Effects")
	bool Sine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Haptic Config/Supported Effects")
	bool Triangle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Haptic Config/Supported Effects")
	bool SawToothUp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Haptic Config/Supported Effects")
	bool SawToothDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Haptic Config/Supported Effects")
	bool Ramp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Haptic Config/Supported Effects")
	bool Spring;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Haptic Config/Supported Effects")
	bool Damper;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Haptic Config/Supported Effects")
	bool Inertia;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Haptic Config/Supported Effects")
	bool Friction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Haptic Config/Supported Effects")
	bool Custom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Haptic Config/Supported Effects")
	bool Gain;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Haptic Config/Supported Effects")
	bool AutoCenter;
};
