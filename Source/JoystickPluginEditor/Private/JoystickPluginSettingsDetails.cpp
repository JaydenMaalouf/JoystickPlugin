// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickPluginSettingsDetails.h"

#include "Data/Settings/JoystickInputDeviceConfiguration.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
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

	IDetailCategoryBuilder& JoystickCategory = DetailBuilder.EditCategory("Information");
	JoystickCategory.AddCustomRow(LOCTEXT("Joystick", "Joystick"))
	                .ValueContent()
	[SNew(SHorizontalBox)
	+ SHorizontalBox::Slot()
	.Padding(5)
	.AutoWidth()
	[
		SNew(SButton)
		.Text(LOCTEXT("AddDevices", "Create Configurations for Connected Devices"))
		.ToolTipText(LOCTEXT(
			"AddDevices_Tooltip", "Create a Device Configuration for each of the connected devices."))
		.OnClicked_Lambda([this, Settings]
		{
			for (const FJoystickInformation& ConnectedDevice : Settings->ConnectedDevices)
			{
				Settings->AddDeviceConfiguration(FJoystickInputDeviceConfiguration(ConnectedDevice.ProductGuid));
			}

			return FReply::Handled();
		})
	]
	+ SHorizontalBox::Slot()
	.Padding(5)
	.AutoWidth()
	[
		SNew(SButton)
		.Text(LOCTEXT("OpenVisualizer", "Open Joystick Viewer"))
		.ToolTipText(LOCTEXT(
			"OpenVisualizer_Tooltip", "Opens the Joystick Viewer debugging window."))
		.OnClicked_Lambda([this]
		{
			FGlobalTabmanager::Get()->TryInvokeTab(FName("JoystickInputViewer"));
			return FReply::Handled();
		})
	]];
}

#undef LOCTEXT_NAMESPACE
