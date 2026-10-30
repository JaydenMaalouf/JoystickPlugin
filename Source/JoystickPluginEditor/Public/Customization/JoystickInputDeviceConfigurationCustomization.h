// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#if WITH_EDITOR

#include "IPropertyTypeCustomization.h"

struct FJoystickInputDeviceConfiguration;

class FJoystickInputDeviceConfigurationCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FJoystickInputDeviceConfigurationCustomization);
	}

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

private:
	bool IsDeviceConfigurationArrayItem(const TSharedRef<IPropertyHandle>& StructPropertyHandle) const;
	FText GetConnectedDeviceDisplayName(const TSharedRef<IPropertyHandle>& StructPropertyHandle) const;
	FReply ConvertToProfile(const TSharedRef<IPropertyHandle>& StructPropertyHandle) const;
	FString ResolveProfileFileName(const FJoystickInputDeviceConfiguration& DeviceConfiguration) const;
	bool ConfirmProfileOverwrite(const FText& Message) const;
	FString PromptForProfileName(const FString& InitialProfileName) const;
	const FJoystickInputDeviceConfiguration* GetDeviceConfiguration(const TSharedRef<IPropertyHandle>& StructPropertyHandle) const;
};

#endif
