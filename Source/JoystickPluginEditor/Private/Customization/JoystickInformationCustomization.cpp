// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Customization/JoystickInformationCustomization.h"

#if WITH_EDITOR

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "JoystickInputSettings.h"
#include "Styling/JoystickPluginEditorStyle.h"

void FJoystickInformationCustomization::CustomizeHeader(const TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	if (!IsConnectedDeviceArrayItem(StructPropertyHandle))
	{
		return;
	}

	HeaderRow
		.NameContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				StructPropertyHandle->CreatePropertyNameWidget()
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(8.0f, 0.0f, 0.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text_Lambda([this, StructPropertyHandle]
				{
					return GetConnectedDeviceDisplayName(StructPropertyHandle);
				})
				.TextStyle(FJoystickPluginEditorStyle::Get(), "HintText")
			]
		]
		.ValueContent()
		.MinDesiredWidth(350.0f);
}

void FJoystickInformationCustomization::CustomizeChildren(const TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	uint32 ChildCount = 0;
	StructPropertyHandle->GetNumChildren(ChildCount);
	for (uint32 ChildIndex = 0; ChildIndex < ChildCount; ++ChildIndex)
	{
		if (TSharedPtr<IPropertyHandle> ChildHandle = StructPropertyHandle->GetChildHandle(ChildIndex))
		{
			StructBuilder.AddProperty(ChildHandle.ToSharedRef());
		}
	}
}

bool FJoystickInformationCustomization::IsConnectedDeviceArrayItem(const TSharedRef<IPropertyHandle>& StructPropertyHandle) const
{
	TSharedPtr<IPropertyHandle> ParentHandle = StructPropertyHandle->GetParentHandle();
	while (ParentHandle.IsValid())
	{
		if (const FProperty* ParentProperty = ParentHandle->GetProperty())
		{
			if (ParentProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UJoystickInputSettings, ConnectedDevices))
			{
				return true;
			}
		}

		ParentHandle = ParentHandle->GetParentHandle();
	}

	return false;
}

FText FJoystickInformationCustomization::GetConnectedDeviceDisplayName(const TSharedRef<IPropertyHandle>& StructPropertyHandle) const
{
	const UJoystickInputSettings* InputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!InputSettings)
	{
		return FText::GetEmpty();
	}

	const FJoystickInformation* ConnectedDevice = GetJoystickInformation(StructPropertyHandle);
	if (!ConnectedDevice)
	{
		return FText::GetEmpty();
	}

	return FText::FromString(ConnectedDevice->GetDeviceDisplayName());
}

const FJoystickInformation* FJoystickInformationCustomization::GetJoystickInformation(const TSharedRef<IPropertyHandle>& StructPropertyHandle) const
{
	TArray<void*> RawData;
	StructPropertyHandle->AccessRawData(RawData);
	if (RawData.Num() != 1 || RawData[0] == nullptr)
	{
		return nullptr;
	}
	return static_cast<FJoystickInformation*>(RawData[0]);
}

#endif
