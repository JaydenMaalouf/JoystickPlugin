#pragma once

#include "Data/InputType.h"
#include "Containers/Array.h"

#include "JoystickInfo.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickInfo)
		int32 Player = -1;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickInfo)
		int32 DeviceId = -1;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickInfo)
		bool IsRumbleDevice = false;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickInfo)
		FGuid ProductId;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickInfo)
		FString ProductName;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickInfo)
		FString DeviceName;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickInfo)
		bool Connected = false;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = JoystickInfo)
		TArray<EInputType> InputType;
};