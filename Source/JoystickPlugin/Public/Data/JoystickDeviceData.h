#pragma once

#include "Containers/Array.h"
#include "Data/Input/AnalogData.h"
#include "Data/Input/BallData.h"
#include "Data/Input/ButtonData.h"
#include "Data/Input/HatData.h"

#include "JoystickDeviceData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickDeviceData
{
	GENERATED_BODY()

	explicit FJoystickDeviceData(int32 DeviceId = -1) : DeviceId(DeviceId)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickState)
		int32 DeviceId;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickState)
		int32 VendorId;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickState)
		FString DeviceName;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickState)
		TArray<FAnalogData> Axes;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickState)
		TArray<FButtonData> Buttons;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickState)
		TArray<FHatData> Hats;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickState)
		TArray<FBallData> Balls;
};