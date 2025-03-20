// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickPluginEditorModule.h"

#include "ISettingsContainer.h"
#include "ISettingsModule.h"
#include "JoystickInputSettings.h"
#include "JoystickPluginSettingsDetails.h"
#include "Customization/JoystickInstanceIdCustomization.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "PropertyEditorDelegates.h"

#define LOCTEXT_NAMESPACE "JoystickPluginEditor"

void FJoystickPluginEditorModule::StartupModule()
{
	RegisterSettings();

	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(UJoystickInputSettings::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FJoystickPluginSettingsDetails::MakeInstance));
	//PropertyModule.RegisterCustomPropertyTypeLayout("JoystickInstanceId", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FJoystickInstanceIdCustomization::MakeInstance));

	IModuleInterface::StartupModule();
}

void FJoystickPluginEditorModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

void FJoystickPluginEditorModule::RegisterSettings() const
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Engine", "Joystick Input",
		                                 LOCTEXT("JoystickInputSettingsName", "Joystick Input"),
		                                 LOCTEXT("JoystickInputSettingsDescription", "Configure Joystick Input"), GetMutableDefault<UJoystickInputSettings>());
	}
}

void FJoystickPluginEditorModule::UnregisterSettings() const
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Engine", "Joystick Input");
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FJoystickPluginEditorModule, JoystickPluginEditor)
