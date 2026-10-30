// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "JoystickPluginEditorModule.h"

#include "ContentBrowserMenuContexts.h"
#include "ISettingsModule.h"
#include "JoystickInputSettings.h"
#include "JoystickPluginSettingsDetails.h"
#include "PropertyEditorDelegates.h"
#include "PropertyEditorModule.h"
#include "ToolMenus.h"
#include "Customization/ForceFeedbackEffectDetails.h"
#include "Customization/JoystickInformationCustomization.h"
#include "Customization/JoystickInputDeviceConfigurationCustomization.h"
#include "Customization/JoystickInstanceIdCustomization.h"
#include "Data/Settings/JoystickInputDeviceConfiguration.h"
#include "ForceFeedback/Data/ForceFeedbackEffectGroup.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "Menus/HapticEffectEditor.h"
#include "Menus/JoystickInputViewer.h"
#include "Modules/ModuleManager.h"
#include "Styling/JoystickPluginEditorStyle.h"

#define LOCTEXT_NAMESPACE "JoystickPluginEditor"

static const FName JoystickViewerTabId(TEXT("JoystickInputViewer"));
static const FName HapticEffectEditorTabId(TEXT("HapticEffectEditor"));

// Brings up the Haptic Effect Editor tab (opening it if need be) and loads the asset into it.
static void EditAssetWithHapticEffectEditor(const FAssetData AssetData, const int32 EntryIndex)
{
	const TSharedPtr<SDockTab> DockTab = FGlobalTabmanager::Get()->TryInvokeTab(HapticEffectEditorTabId);
	if (!DockTab.IsValid())
	{
		return;
	}

	// The tab's content is always the SHapticEffectEditor its spawner (see StartupModule) gave it.
	StaticCastSharedRef<SHapticEffectEditor>(DockTab->GetContent())->EditAsset(AssetData.GetAsset(), EntryIndex);
}

void FJoystickPluginEditorModule::StartupModule()
{
	FJoystickPluginEditorStyle::Register();

	RegisterSettings();
	RegisterPropertyLayout();

	FGlobalTabmanager::Get()
		->RegisterNomadTabSpawner(
			JoystickViewerTabId,
			FOnSpawnTab::CreateLambda([](const FSpawnTabArgs& SpawnTabArgs) -> TSharedRef<SDockTab>
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

	FGlobalTabmanager::Get()
		->RegisterNomadTabSpawner(
			HapticEffectEditorTabId,
			FOnSpawnTab::CreateLambda([](const FSpawnTabArgs& SpawnTabArgs) -> TSharedRef<SDockTab>
			{
				const TSharedRef<SDockTab> DockTab =
					SNew(SDockTab)
					.TabRole(NomadTab);

				const TSharedRef<SHapticEffectEditor> Editor = SNew(SHapticEffectEditor);
				DockTab->SetContent(Editor);
				DockTab->SetLabel(TAttribute<FText>::CreateSP(&Editor.Get(), &SHapticEffectEditor::GetTabLabel));

				// Asked both when the tab itself is closed and when the whole editor is.
				DockTab->SetCanCloseTab(SDockTab::FCanCloseTab::CreateSP(Editor, &SHapticEffectEditor::PromptToSaveChanges));
				return DockTab;
			})
		)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.PadEvent_16x"))
		.SetDisplayName(FText::FromString("Haptic Effect Editor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FJoystickPluginEditorModule::RegisterMenus));

	IModuleInterface::StartupModule();
}

void FJoystickPluginEditorModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterTabSpawner(JoystickViewerTabId);
	FGlobalTabmanager::Get()->UnregisterTabSpawner(HapticEffectEditorTabId);

	if (UToolMenus::IsToolMenuUIEnabled())
	{
		UToolMenus::UnRegisterStartupCallback(this);
	}

	UnregisterSettings();
	UnregisterPropertyLayout();

	FJoystickPluginEditorStyle::Unregister();

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
			FGlobalTabmanager::Get()->TryInvokeTab(JoystickViewerTabId);
		}))
	);
	Section.AddMenuEntry(
		"OpenHapticEffectEditor",
		FText::FromString("Haptic Effect Editor"),
		FText::FromString("Open the Haptic Effect Editor tab."),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.PadEvent_16x"),
		FUIAction(FExecuteAction::CreateLambda([]
		{
			FGlobalTabmanager::Get()->TryInvokeTab(HapticEffectEditorTabId);
		}))
	);

	// Adds "Edit with Haptic Effect Editor" to the content browser's asset context menu.
	UToolMenu* AssetContextMenu = UToolMenus::Get()->ExtendMenu("ContentBrowser.AssetContextMenu");
	FToolMenuSection& AssetActionsSection = AssetContextMenu->FindOrAddSection("GetAssetActions");
	AssetActionsSection.AddDynamicEntry("EditWithHapticEffectEditor", FNewToolMenuSectionDelegate::CreateLambda([](FToolMenuSection& InSection)
	{
		const UContentBrowserAssetContextMenuContext* Context = InSection.FindContext<UContentBrowserAssetContextMenuContext>();
		if (!Context || Context->SelectedAssets.Num() != 1 || !SHapticEffectEditor::CanEditAsset(Context->SelectedAssets[0]))
		{
			return;
		}

		const FAssetData AssetData = Context->SelectedAssets[0];
		const FText Label = LOCTEXT("EditWithHapticEffectEditor", "Edit with Haptic Effect Editor");
		const FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "GraphEditor.PadEvent_16x");

		const UClass* AssetClass = AssetData.GetClass();
		if (!AssetClass || !AssetClass->IsChildOf(UForceFeedbackEffectGroup::StaticClass()))
		{
			InSection.AddMenuEntry(
				"EditWithHapticEffectEditor",
				Label,
				LOCTEXT("EditBlueprintWithHapticEffectEditorTooltip", "Open this effect's default values in the Haptic Effect Editor."),
				Icon,
				FUIAction(FExecuteAction::CreateStatic(&EditAssetWithHapticEffectEditor, AssetData, static_cast<int32>(INDEX_NONE))));
			return;
		}

		// A group holds several effects, so the entry is picked from a submenu (the group only loads once it is opened).
		InSection.AddSubMenu(
			"EditWithHapticEffectEditor",
			Label,
			LOCTEXT("EditGroupWithHapticEffectEditorTooltip", "Open one of this effect group's entries in the Haptic Effect Editor."),
			FNewToolMenuDelegate::CreateLambda([AssetData](UToolMenu* SubMenu)
			{
				FToolMenuSection& EntriesSection = SubMenu->AddSection("Entries", LOCTEXT("EffectGroupEntriesSection", "Entries"));

				TArray<int32> EditableEntries;
				const UForceFeedbackEffectGroup* EffectGroup = Cast<UForceFeedbackEffectGroup>(AssetData.GetAsset());
				if (EffectGroup)
				{
					SHapticEffectEditor::GetEditableEntries(*EffectGroup, EditableEntries);
				}

				if (EditableEntries.Num() == 0)
				{
					EntriesSection.AddMenuEntry(
						"NoEditableEntries",
						LOCTEXT("NoEditableEntries", "No periodic, constant, ramp or left/right effects"),
						FText::GetEmpty(),
						FSlateIcon(),
						FUIAction(FExecuteAction(), FCanExecuteAction::CreateLambda([] { return false; })));
					return;
				}

				for (const int32 EntryIndex : EditableEntries)
				{
					const UForceFeedbackEffectBase* EntryEffect = EffectGroup->Entries[EntryIndex].Effect;
					EntriesSection.AddMenuEntry(
						FName(*FString::Printf(TEXT("Entry%d"), EntryIndex)),
						SHapticEffectEditor::GetEntryLabel(*EffectGroup, EntryIndex),
						EntryEffect->GetClass()->GetDisplayNameText(),
						FSlateIcon(),
						FUIAction(FExecuteAction::CreateStatic(&EditAssetWithHapticEffectEditor, AssetData, EntryIndex)));
				}
			}),
			false,
			Icon);
	}));
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

void FJoystickPluginEditorModule::RegisterPropertyLayout() const
{
	if (FPropertyEditorModule* PropertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		PropertyModule->RegisterCustomClassLayout(UJoystickInputSettings::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FJoystickPluginSettingsDetails::MakeInstance));
		PropertyModule->RegisterCustomClassLayout(UForceFeedbackEffectBase::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FForceFeedbackEffectDetails::MakeInstance));
		PropertyModule->RegisterCustomPropertyTypeLayout(FJoystickInputDeviceConfiguration::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FJoystickInputDeviceConfigurationCustomization::MakeInstance));
		PropertyModule->RegisterCustomPropertyTypeLayout(FJoystickInformation::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FJoystickInformationCustomization::MakeInstance));
		PropertyModule->RegisterCustomPropertyTypeLayout(FJoystickInstanceId::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FJoystickInstanceIdCustomization::MakeInstance));
	}
}

void FJoystickPluginEditorModule::UnregisterSettings() const
{
	if (UObjectInitialized() == false)
	{
		return;
	}

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Engine", SettingsSection);
	}
}

void FJoystickPluginEditorModule::UnregisterPropertyLayout() const
{
	if (UObjectInitialized() == false)
	{
		return;
	}

	if (FPropertyEditorModule* PropertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		PropertyModule->UnregisterCustomClassLayout(UJoystickInputSettings::StaticClass()->GetFName());
		PropertyModule->UnregisterCustomClassLayout(UForceFeedbackEffectBase::StaticClass()->GetFName());
		PropertyModule->UnregisterCustomPropertyTypeLayout(FJoystickInputDeviceConfiguration::StaticStruct()->GetFName());
		PropertyModule->UnregisterCustomPropertyTypeLayout(FJoystickInformation::StaticStruct()->GetFName());
		PropertyModule->UnregisterCustomPropertyTypeLayout(FJoystickInstanceId::StaticStruct()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FJoystickPluginEditorModule, JoystickPluginEditor)
