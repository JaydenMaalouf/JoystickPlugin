// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once
#include "Data/Settings/JoystickInputDeviceButtonProperties.h"

#include "ButtonData.generated.h"

// A button's state, with the inversion its configuration applies on read.
USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FButtonData
{
	GENERATED_BODY()

	FButtonData()
		: ButtonState(false)
		  , PreviousButtonState(false)
		  , InvertOutput(false)
		  , Timestamp(0)
	{
	}

	bool GetValue() const
	{
		return InvertOutput ? !ButtonState : ButtonState;
	}

	// The current state as the given properties would invert it, for previewing a configuration before it is applied.
	bool GetMockValue(const FJoystickInputDeviceButtonProperties& ButtonProperties) const
	{
		return ButtonProperties.InvertOutput ? !ButtonState : ButtonState;
	}

	bool GetPreviousValue() const
	{
		return InvertOutput ? !PreviousButtonState : PreviousButtonState;
	}

	void Update(const bool& InValue, const int64 InTimestamp)
	{
		PreviousButtonState = ButtonState;
		ButtonState = InValue;
		Timestamp = InTimestamp;
	}

	// Marks the current value as handled, so it no longer reads as changed from the previous one.
	void Processed()
	{
		PreviousButtonState = ButtonState;
	}

	// Whether the button is held, before inversion.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Button")
	bool ButtonState;

	// Whether the button was held before the last update, before inversion.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Button")
	bool PreviousButtonState;

	// Inverts the state on read, so a held button reads as released.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Button")
	bool InvertOutput;

	// When SDL read the current value, in nanoseconds.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Button")
	int64 Timestamp;
};
