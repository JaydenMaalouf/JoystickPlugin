#pragma once

#include "Containers/Array.h"
#include "Data/Input/AxisData.h"
#include "Data/Input/BallData.h"
#include "Data/Input/ButtonData.h"
#include "Data/Input/HatData.h"

#include "JoystickDeviceData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickDeviceData
{
	GENERATED_BODY()

	explicit FJoystickDeviceData(int32 DeviceId = -1)
		: DeviceId(DeviceId)
		, VendorId(-1)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickState)
		int32 DeviceId;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickState)
		int32 VendorId;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickState)
		FString DeviceName;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickState)
		TArray<FAxisData> Axes;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickState)
		TArray<FButtonData> Buttons;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickState)
		TArray<FHatData> Hats;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickState)
		TArray<FBallData> Balls;
};