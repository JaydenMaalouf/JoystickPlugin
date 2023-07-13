// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickPluginSettingsDetails.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "JoystickInputSettings.h"
#include "Widgets/Input/SButton.h"

#define LOCTEXT_NAMESPACE "JoystickPluginSettingsDetails"

TSharedRef<IDetailCustomization> FJoystickPluginSettingsDetails::MakeInstance()
{
	return MakeShareable(new FJoystickPluginSettingsDetails);
}

void FJoystickPluginSettingsDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
	DetailBuilder.GetObjectsBeingCustomized(ObjectsBeingCustomized);
	check(ObjectsBeingCustomized.Num() == 1);
	TWeakObjectPtr<UJoystickInputSettings> Settings = Cast<UJoystickInputSettings>(ObjectsBeingCustomized[0].Get());

	IDetailCategoryBuilder& EncryptionCategory = DetailBuilder.EditCategory("Information");
	FDetailWidgetRow& a = EncryptionCategory.AddCustomRow(LOCTEXT("EncryptionKeyGenerator", "EncryptionKeyGenerator"))
	                                        .ValueContent()
	[SNew(SHorizontalBox) + SHorizontalBox::Slot()
	                        .Padding(5)
	                        .AutoWidth()
		[SNew(SButton)
																																		 .Text(LOCTEXT("AddDevices", "Create Configurations for Connected Devices"))
																																		 .ToolTipText(LOCTEXT(
			              "AddDevices_Tooltip", "Create a Device Configuration for each of the connected devices."))
																																		 .OnClicked_Lambda([this, Settings]()
		              {
			              for (const FJoystickInformation& ConnectedDevice : Settings->ConnectedDevices)
			              {
				              if (Settings->DeviceConfigurations.FindByPredicate([&](const FJoystickInputDeviceConfiguration& DeviceConfiguration)
				              {
					              return DeviceConfiguration.ProductGuid == ConnectedDevice.ProductGuid;
				              }))
				              {
					              continue;
				              }

				              Settings->DeviceConfigurations.Add(FJoystickInputDeviceConfiguration(ConnectedDevice.ProductGuid));
			              }

			              return (FReply::Handled());
		              })]];
}

#undef LOCTEXT_NAMESPACE
