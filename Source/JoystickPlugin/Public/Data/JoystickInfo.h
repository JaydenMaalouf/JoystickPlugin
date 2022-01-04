#pragma once

#include "Data/InputType.h"
#include "Containers/Array.h"

#include "JoystickInfo.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickInfo
{
	GENERATED_BODY()

	FJoystickInfo()
		: Player(-1)
		, DeviceId(-1)
		, IsRumbleDevice(false)
		, Connected(false)
	{
		
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
		int32 Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
		int32 DeviceId;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
		bool IsRumbleDevice;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
		FGuid ProductId;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
		FString ProductName;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
		FString DeviceName;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
		bool Connected;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
		TArray<EInputType> InputType;
};