// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "JoystickPluginEditorModule.h"

#include "BlueprintEditor.h"
#include "BlueprintEditorModes.h"
#include "BlueprintEditorModule.h"
#include "BlueprintEditorTabs.h"
#include "ISettingsModule.h"
#include "JoystickPluginEditorTabIds.h"
#include "JoystickInputSettings.h"
#include "Menus/JoystickInputViewer.h"
#include "Customization/JoystickInstanceIdCustomization.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "PropertyEditorDelegates.h"
#include "Tabs/ForceFeedbackPreviewTabFactory.h"
#include "ToolMenus.h"
#include "Customization/JoystickSettingsDetailsCustomization.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "Framework/Docking/LayoutExtender.h"

#include "Framework/Docking/TabManager.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

#define LOCTEXT_NAMESPACE "JoystickPluginEditor"

void FJoystickPluginEditorModule::StartupModule()
{
	RegisterSettings();
	RegisterPropertyLayout();

	// Extend the Blueprint editor (ForceFeedbackEffect blueprints) with a preview tab.
	{
		FBlueprintEditorModule& BlueprintEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
		BlueprintTabsHandle = BlueprintEditorModule.OnRegisterTabsForEditor().AddRaw(this, &FJoystickPluginEditorModule::RegisterBlueprintEditorTabs);
		BlueprintLayoutHandle = BlueprintEditorModule.OnRegisterLayoutExtensions().AddRaw(this, &FJoystickPluginEditorModule::RegisterBlueprintEditorLayout);
	}

	FGlobalTabmanager::Get()
		->RegisterNomadTabSpawner(
			JoystickPluginEditorTabIds::JoystickViewer,
			FOnSpawnTab::CreateLambda([&](const FSpawnTabArgs& SpawnTabArgs) -> TSharedRef<SDockTab>
			{
				const TSharedRef<SDockTab> DockTab =
					SNew(SDockTab)
					.TabRole(NomadTab);

				const TSharedRef<SJoystickInputViewer> Frontend =
					SNew(SJoystickInputViewer, DockTab, SpawnTabArgs.GetOwnerWindow());

				DockTab->SetContent(Frontend);
				return DockTab;
			})
		)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.PadEvent_16x"))
		.SetDisplayName(FText::FromString("Joystick Viewer"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FJoystickPluginEditorModule::RegisterMenus));

	IModuleInterface::StartupModule();
}

void FJoystickPluginEditorModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterTabSpawner(JoystickPluginEditorTabIds::JoystickViewer);

	// Unhook Blueprint editor extensions.
	if (FBlueprintEditorModule* BlueprintEditorModule = FModuleManager::GetModulePtr<FBlueprintEditorModule>("Kismet"))
	{
		if (BlueprintTabsHandle.IsValid())
		{
			BlueprintEditorModule->OnRegisterTabsForEditor().Remove(BlueprintTabsHandle);
			BlueprintTabsHandle.Reset();
		}
		if (BlueprintLayoutHandle.IsValid())
		{
			BlueprintEditorModule->OnRegisterLayoutExtensions().Remove(BlueprintLayoutHandle);
			BlueprintLayoutHandle.Reset();
		}
	}

	if (UToolMenus::IsToolMenuUIEnabled())
	{
		UToolMenus::UnRegisterStartupCallback(this);
	}

	UnregisterSettings();

	IModuleInterface::ShutdownModule();
}

void FJoystickPluginEditorModule::RegisterMenus() const
{
	UToolMenu* ToolsMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
	FToolMenuSection& Section = ToolsMenu->FindOrAddSection("Tools");
	Section.AddMenuEntry(
		"OpenJoystickViewer",
		FText::FromString("Joystick Viewer"),
		FText::FromString("Open the Joystick Input Viewer tab."),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.PadEvent_16x"),
		FUIAction(FExecuteAction::CreateLambda([]
		{
			FGlobalTabmanager::Get()->TryInvokeTab(JoystickPluginEditorTabIds::JoystickViewer);
		}))
	);
}

void FJoystickPluginEditorModule::RegisterPropertyLayout() const
{
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(UJoystickInputSettings::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FJoystickSettingsDetailsCustomization::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout("JoystickInstanceId", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FJoystickInstanceIdCustomization::MakeInstance));
}

void FJoystickPluginEditorModule::RegisterBlueprintEditorTabs(FWorkflowAllowedTabSet& TabFactories, FName ModeName, TSharedPtr<FBlueprintEditor> BlueprintEditor) const
{
	if (!BlueprintEditor.IsValid())
	{
		return;
	}

	if (ModeName != FBlueprintEditorApplicationModes::StandardBlueprintEditorMode)
	{
		return;
	}

	const UBlueprint* Blueprint = BlueprintEditor->GetBlueprintObj();
	if (!IsValid(Blueprint) || !IsValid(Blueprint->ParentClass))
	{
		return;
	}

	// Only show the tab for Force Feedback effect blueprints.
	if (!Blueprint->ParentClass->IsChildOf(UForceFeedbackEffectBase::StaticClass()))
	{
		return;
	}

	if (!TabFactories.GetFactory(JoystickPluginEditorTabIds::ForceFeedbackPreview).IsValid())
	{
		TabFactories.RegisterFactory(MakeShared<FForceFeedbackPreviewTabFactory>(BlueprintEditor));
	}
}

void FJoystickPluginEditorModule::RegisterBlueprintEditorLayout(FLayoutExtender& Extender) const
{
	// Put our tab next to "My Blueprint" (same tab stack).
	// If the tab isn't registered for a given blueprint, it won't appear.
	Extender.ExtendLayout(
		FTabId(FBlueprintEditorTabs::MyBlueprintID),
		ELayoutExtensionPosition::Below,
		FTabManager::FTab(JoystickPluginEditorTabIds::ForceFeedbackPreview, ETabState::ClosedTab)
	);
}

static FName SettingsSection = TEXT("Joystick Input");

void FJoystickPluginEditorModule::RegisterSettings() const
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Engine", SettingsSection,
		                                 LOCTEXT("JoystickInputSettingsName", "Joystick Input"),
		                                 LOCTEXT("JoystickInputSettingsDescription", "Configure Joystick Input"), GetMutableDefault<UJoystickInputSettings>());
	}
}

void FJoystickPluginEditorModule::UnregisterSettings() const
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Engine", SettingsSection);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FJoystickPluginEditorModule, JoystickPluginEditor)
