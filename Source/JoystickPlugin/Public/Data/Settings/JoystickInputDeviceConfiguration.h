// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "JoystickInputDeviceAxisProperties.h"
#include "JoystickInputDeviceButtonProperties.h"

#include "Data/JoystickIdentifierType.h"

#include "JoystickInputDeviceConfiguration.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickInputDeviceConfiguration
{
	GENERATED_BODY()

	FJoystickInputDeviceConfiguration()
		: DeviceIdentifyMethod(EJoystickIdentifierType::Hashed)
		  , OverrideDeviceName(false)
	{
	}

	FJoystickInputDeviceConfiguration(const FGuid& JoystickProductId)
		: DeviceIdentifyMethod(EJoystickIdentifierType::Hashed)
		  , ProductGuid(JoystickProductId)
		  , OverrideDeviceName(false)
	{
	}

	FJoystickInputDeviceConfiguration(const FString& JoystickHash)
		: DeviceIdentifyMethod(EJoystickIdentifierType::Hashed)
		  , DeviceHash(JoystickHash)
		  , OverrideDeviceName(false)
	{
	}

	UPROPERTY(EditAnywhere, Category="Joystick Settings",
		meta=(ToolTip="Method to identify a joystick to apply configurations to.", ConfigRestartRequired=true))
	EJoystickIdentifierType DeviceIdentifyMethod;

	UPROPERTY(EditAnywhere, Category="Device Config", meta=(ToolTip="Used to determine the device to apply the configuration to.", EditCondition="DeviceIdentifyMethod == EJoystickIdentifierType::Hashed", EditConditionHides))
	FString DeviceHash;

	UPROPERTY(EditAnywhere, Category="Device Config", meta=(ToolTip="Used to determine the device to apply the configuration to.", EditCondition="DeviceIdentifyMethod == EJoystickIdentifierType::Legacy", EditConditionHides))
	FGuid ProductGuid;

	UPROPERTY(EditAnywhere, Category="Device Config",
		meta=(ToolTip="If enabled and the global setting for using Device Names as input, it will use the name specified as part of the key name. Requires a restart to apply.", ConfigRestartRequired=true))
	bool OverrideDeviceName;

	UPROPERTY(EditAnywhere, Category="Device Config", meta=(EditCondition="OverrideDeviceName", EditConditionHides), NoClear)
	FString DeviceName;

	UPROPERTY(EditAnywhere, Category="Device Config", meta=(TitleProperty="AxisIndex"))
	TArray<FJoystickInputDeviceAxisProperties> AxisProperties;

	UPROPERTY(EditAnywhere, Category="Device Config", meta=(TitleProperty="ButtonIndex"))
	TArray<FJoystickInputDeviceButtonProperties> ButtonProperties;

	FJoystickInputDeviceAxisProperties* GetAxisProperties(const int AxisIndex)
	{
		return AxisProperties.FindByPredicate([AxisIndex](const FJoystickInputDeviceAxisProperties& AxisProperty)
		{
			return AxisProperty.AxisIndex != -1 && AxisProperty.AxisIndex == AxisIndex;
		});
	}

	FJoystickInputDeviceButtonProperties* GetButtonProperties(const int ButtonIndex)
	{
		return ButtonProperties.FindByPredicate([ButtonIndex](const FJoystickInputDeviceButtonProperties& ButtonProperty)
		{
			return ButtonProperty.ButtonIndex != -1 && ButtonProperty.ButtonIndex == ButtonIndex;
		});
	}

	void Merge(FJoystickInputDeviceConfiguration& Other)
	{
		OverrideDeviceName = Other.OverrideDeviceName;
		DeviceName = Other.DeviceName;

		for (FJoystickInputDeviceAxisProperties& AxisProperty : AxisProperties)
		{
			const FJoystickInputDeviceAxisProperties* OtherAxisProperties = Other.GetAxisProperties(AxisProperty.AxisIndex);
			if (!OtherAxisProperties)
			{
				continue;
			}

			AxisProperty.UpdateProperties(*OtherAxisProperties);
		}

		for (FJoystickInputDeviceButtonProperties& ButtonProperty : ButtonProperties)
		{
			const FJoystickInputDeviceButtonProperties* OtherButtonProperties = Other.GetButtonProperties(ButtonProperty.ButtonIndex);
			if (!OtherButtonProperties)
			{
				continue;
			}

			ButtonProperty.UpdateProperties(*OtherButtonProperties);
		}
	}
};
