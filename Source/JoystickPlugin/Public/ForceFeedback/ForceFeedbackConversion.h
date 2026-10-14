// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

THIRD_PARTY_INCLUDES_START

#include "JoystickSDL.h"

THIRD_PARTY_INCLUDES_END

// Conversions from user facing units to the integer ranges SDL expects, clamped so out of range values saturate instead of wrapping.
struct FForceFeedbackConversion
{
	// Seconds to a Uint32 length in milliseconds.
	static uint32 SecondsToDuration(const double Seconds)
	{
		return static_cast<uint32>(FMath::Clamp(FMath::RoundToDouble(Seconds * 1000.0), 0.0, static_cast<double>(SDL_HAPTIC_INFINITY)));
	}

	// Seconds to a Uint16 time in milliseconds (delay, interval, period, attack and fade length), capped at 32.767 seconds.
	static uint16 SecondsToMilliseconds(const double Seconds)
	{
		return static_cast<uint16>(FMath::RoundToInt32(FMath::Clamp(Seconds * 1000.0, 0.0, static_cast<double>(INT16_MAX))));
	}

	// -1..1 to a Sint16 level of -32767..32767.
	static int16 ToSignedLevel(const double Value)
	{
		return static_cast<int16>(FMath::RoundToInt32(FMath::Clamp(Value, -1.0, 1.0) * INT16_MAX));
	}

	// 0..1 to a Uint16 level of 0..32767, for fields where SDL only uses the positive half of the range.
	static uint16 ToUnsignedLevel(const double Value)
	{
		return static_cast<uint16>(FMath::RoundToInt32(FMath::Clamp(Value, 0.0, 1.0) * INT16_MAX));
	}

	// 0..1 to a Uint16 level of 0..65535, for fields where SDL uses the full range.
	static uint16 ToFullRangeLevel(const double Value)
	{
		return static_cast<uint16>(FMath::RoundToInt32(FMath::Clamp(Value, 0.0, 1.0) * UINT16_MAX));
	}

	// Degrees to a Uint16 phase in hundredths of a degree, 0..36000.
	static uint16 DegreesToPhase(const double Degrees)
	{
		return static_cast<uint16>(FMath::RoundToInt32(FMath::Clamp(Degrees, 0.0, 360.0) * 100.0));
	}

	// A direction angle in hundredths of a degree, kept within 0..36000.
	static int32 ToDirectionAngle(const int32 HundredthsOfDegrees)
	{
		return FMath::Clamp(HundredthsOfDegrees, 0, 36000);
	}

	// A direction elevation in hundredths of a degree, kept within -9000..9000.
	static int32 ToDirectionElevation(const int32 HundredthsOfDegrees)
	{
		return FMath::Clamp(HundredthsOfDegrees, -9000, 9000);
	}

	// A custom effect sample count to a Uint16 of 0..65535.
	static uint16 ToSampleCount(const int32 Count)
	{
		return static_cast<uint16>(FMath::Clamp(Count, 0, static_cast<int32>(UINT16_MAX)));
	}

	// A custom effect sample to a Uint16 level of 0..32767.
	static uint16 ToSample(const int32 Value)
	{
		return static_cast<uint16>(FMath::Clamp(Value, 0, static_cast<int32>(INT16_MAX)));
	}

	// A gain or autocenter percentage, kept within 0..100.
	static int32 ToPercentage(const int32 Value)
	{
		return FMath::Clamp(Value, 0, 100);
	}

	// A haptic rumble strength, kept within 0..1.
	static float ToStrength(const float Value)
	{
		return FMath::Clamp(Value, 0.0f, 1.0f);
	}
};
