// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickPluginEditorModule.h"

#include "ISettingsModule.h"
#include "JoystickInputSettings.h"
#include "JoystickInputViewer.h"
#include "JoystickPluginSettingsDetails.h"
#include "Customization/JoystickInstanceIdCustomization.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "PropertyEditorDelegates.h"

#define LOCTEXT_NAMESPACE "JoystickPluginEditor"

void FJoystickPluginEditorModule::StartupModule()
{
	RegisterSettings();
	RegisterProperyLayout();

	FGlobalTabmanager::Get()
		->RegisterTabSpawner(
			"JoystickInputViewer",
			FOnSpawnTab::CreateLambda([&](const FSpawnTabArgs& SpawnTabArgs) -> TSharedRef<SDockTab>
			{
				const TSharedRef<SDockTab> DockTab = SNew(SDockTab).TabRole(MajorTab);
				const TSharedRef<SJoystickInputViewer> Frontend = SNew(SJoystickInputViewer, DockTab, SpawnTabArgs.GetOwnerWindow());

				DockTab->SetContent(Frontend);

				return DockTab;
			})
		)
		.SetDisplayName(FText::FromString("Joystick Viewer"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	IModuleInterface::StartupModule();
}

void FJoystickPluginEditorModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterTabSpawner("JoystickInputViewer");

	IModuleInterface::ShutdownModule();
}

void FJoystickPluginEditorModule::RegisterProperyLayout()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(UJoystickInputSettings::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FJoystickPluginSettingsDetails::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout("JoystickInstanceId", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FJoystickInstanceIdCustomization::MakeInstance));
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
