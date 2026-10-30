// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "JoystickInputDeviceAxisProperties.h"
#include "JoystickInputDeviceButtonProperties.h"
#include "JoystickInputDeviceForceFeedbackProperties.h"

#include "JoystickInputDeviceConfiguration.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickInputDeviceConfiguration
{
	GENERATED_BODY()

	FJoystickInputDeviceConfiguration()
		: OverrideDeviceName(false)
		  , LimitCollectionIndex(false)
		  , CollectionIndex(INDEX_NONE)
		  , OverrideForceFeedbackProperties(false)
	{
	}

	FJoystickInputDeviceConfiguration(const FString& JoystickHash)
		: DeviceHash(JoystickHash)
		  , OverrideDeviceName(false)
		  , LimitCollectionIndex(false)
		  , CollectionIndex(INDEX_NONE)
		  , OverrideForceFeedbackProperties(false)
	{
	}

	UPROPERTY(EditAnywhere, Category="Device Config", meta=(ToolTip="Used to determine the device to apply the configuration to."))
	FString DeviceHash;

	UPROPERTY(EditAnywhere, Category="Device Config",
		meta=(ToolTip="If enabled and the global setting for using Device Names as input, it will use the name specified as part of the key name. Requires a restart to apply.", ConfigRestartRequired=true))
	bool OverrideDeviceName;

	UPROPERTY(EditAnywhere, Category="Device Config", meta=(EditCondition="OverrideDeviceName", EditConditionHides, ConfigRestartRequired=true), NoClear)
	FString DeviceName;

	UPROPERTY(EditAnywhere, Category="Device Config",
		meta=(ToolTip="If enabled, only the driver collection matching the Collection Index is connected for devices that expose multiple collections. Takes priority over the global collection index prioritisation.", ConfigRestartRequired=
			true))
	bool LimitCollectionIndex;

	UPROPERTY(EditAnywhere, Category="Device Config", meta=(EditCondition="LimitCollectionIndex", EditConditionHides, ClampMin="0", ConfigRestartRequired=true))
	int32 CollectionIndex;

	UPROPERTY(EditAnywhere, Category="Device Config", meta=(TitleProperty="AxisIndex"))
	TArray<FJoystickInputDeviceAxisProperties> AxisProperties;

	UPROPERTY(EditAnywhere, Category="Device Config", meta=(TitleProperty="ButtonIndex"))
	TArray<FJoystickInputDeviceButtonProperties> ButtonProperties;

	UPROPERTY(EditAnywhere, Category="Device Config", meta=(ToolTip="If enabled, these force feedback corrections replace any the device's profile ships with."))
	bool OverrideForceFeedbackProperties;

	UPROPERTY(EditAnywhere, Category="Device Config", meta=(EditCondition="OverrideForceFeedbackProperties", EditConditionHides))
	FJoystickInputDeviceForceFeedbackProperties ForceFeedbackProperties;

	FString ProfileFileName;

	bool IsProfile() const
	{
		return ProfileFileName.IsEmpty() == false;
	}

	bool HasDeviceIdentifier() const
	{
		return DeviceHash.IsEmpty() == false;
	}

	FJoystickInputDeviceAxisProperties* GetAxisProperties(const int AxisIndex)
	{
		return AxisProperties.FindByPredicate([AxisIndex](const FJoystickInputDeviceAxisProperties& AxisProperty)
		{
			return AxisProperty.AxisIndex != INDEX_NONE && AxisProperty.AxisIndex == AxisIndex;
		});
	}

	FJoystickInputDeviceButtonProperties* GetButtonProperties(const int ButtonIndex)
	{
		return ButtonProperties.FindByPredicate([ButtonIndex](const FJoystickInputDeviceButtonProperties& ButtonProperty)
		{
			return ButtonProperty.ButtonIndex != INDEX_NONE && ButtonProperty.ButtonIndex == ButtonIndex;
		});
	}

	FJoystickInputDeviceAxisProperties* FindOrAddAxisProperties(const int AxisIndex)
	{
		if (AxisIndex == INDEX_NONE)
		{
			return nullptr;
		}

		if (FJoystickInputDeviceAxisProperties* ExistingAxisProperties = GetAxisProperties(AxisIndex))
		{
			return ExistingAxisProperties;
		}

		AxisProperties.AddDefaulted_GetRef().AxisIndex = AxisIndex;
		SortAxisProperties();
		return GetAxisProperties(AxisIndex);
	}

	FJoystickInputDeviceButtonProperties* FindOrAddButtonProperties(const int ButtonIndex)
	{
		if (ButtonIndex == INDEX_NONE)
		{
			return nullptr;
		}

		if (FJoystickInputDeviceButtonProperties* ExistingButtonProperties = GetButtonProperties(ButtonIndex))
		{
			return ExistingButtonProperties;
		}

		ButtonProperties.AddDefaulted_GetRef().ButtonIndex = ButtonIndex;
		SortButtonProperties();
		return GetButtonProperties(ButtonIndex);
	}

	void AddOrUpdateAxisProperties(const FJoystickInputDeviceAxisProperties& InAxisProperties)
	{
		FJoystickInputDeviceAxisProperties* TargetAxisProperties = FindOrAddAxisProperties(InAxisProperties.AxisIndex);
		if (TargetAxisProperties == nullptr)
		{
			return;
		}

		TargetAxisProperties->UpdateProperties(InAxisProperties);
	}

	void AddOrUpdateButtonProperties(const FJoystickInputDeviceButtonProperties& InButtonProperties)
	{
		FJoystickInputDeviceButtonProperties* TargetButtonProperties = FindOrAddButtonProperties(InButtonProperties.ButtonIndex);
		if (TargetButtonProperties == nullptr)
		{
			return;
		}

		TargetButtonProperties->UpdateProperties(InButtonProperties);
	}

	void MergeAxisProperties(const FJoystickInputDeviceAxisProperties& InAxisProperties)
	{
		FJoystickInputDeviceAxisProperties* TargetAxisProperties = FindOrAddAxisProperties(InAxisProperties.AxisIndex);
		if (TargetAxisProperties == nullptr)
		{
			return;
		}

		TargetAxisProperties->MergeProperties(InAxisProperties);
	}

	void MergeButtonProperties(const FJoystickInputDeviceButtonProperties& InButtonProperties)
	{
		FJoystickInputDeviceButtonProperties* TargetButtonProperties = FindOrAddButtonProperties(InButtonProperties.ButtonIndex);
		if (TargetButtonProperties == nullptr)
		{
			return;
		}

		TargetButtonProperties->MergeProperties(InButtonProperties);
	}

	void SortAxisProperties()
	{
		AxisProperties.Sort([](const FJoystickInputDeviceAxisProperties& A, const FJoystickInputDeviceAxisProperties& B)
		{
			return A.AxisIndex < B.AxisIndex;
		});
	}

	void SortButtonProperties()
	{
		ButtonProperties.Sort([](const FJoystickInputDeviceButtonProperties& A, const FJoystickInputDeviceButtonProperties& B)
		{
			return A.ButtonIndex < B.ButtonIndex;
		});
	}

	void SortProperties()
	{
		SortAxisProperties();
		SortButtonProperties();
	}

	void Merge(const FJoystickInputDeviceConfiguration& Other)
	{
		if (Other.OverrideDeviceName)
		{
			OverrideDeviceName = true;
			DeviceName = Other.DeviceName;
		}

		if (Other.LimitCollectionIndex)
		{
			LimitCollectionIndex = true;
			CollectionIndex = Other.CollectionIndex;
		}

		for (const FJoystickInputDeviceAxisProperties& OtherAxisProperty : Other.AxisProperties)
		{
			MergeAxisProperties(OtherAxisProperty);
		}

		for (const FJoystickInputDeviceButtonProperties& OtherButtonProperty : Other.ButtonProperties)
		{
			MergeButtonProperties(OtherButtonProperty);
		}

		// Only taken when set, so a configuration made just to rename axes or buttons doesn't clear a profile's corrections.
		if (Other.OverrideForceFeedbackProperties)
		{
			OverrideForceFeedbackProperties = true;
			ForceFeedbackProperties.UpdateProperties(Other.ForceFeedbackProperties);
		}
	}
};
