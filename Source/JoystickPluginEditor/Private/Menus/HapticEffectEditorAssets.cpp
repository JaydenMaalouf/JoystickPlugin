// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

// The half of SHapticEffectEditor that deals with assets: the Open/entry/Save controls, opening an
// effect Blueprint or effect group entry as the working copy, and saving it back or as a new asset.
// Everything else lives in HapticEffectEditor.cpp.

#include "Menus/HapticEffectEditor.h"

#include "ContentBrowserModule.h"
#include "FileHelpers.h"
#include "IContentBrowserSingleton.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Dialogs/HapticEffectAssetTypeDialog.h"
#include "Engine/Blueprint.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "ForceFeedback/Data/ForceFeedbackEffectGroup.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Menus/HapticEffectEditorProperties.h"
#include "Misc/MessageDialog.h"
#include "Misc/PackageName.h"
#include "Modules/ModuleManager.h"
#include "Styling/AppStyle.h"
#include "UObject/Package.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "HapticEffectEditor"

static void NotifyNothingToOpen(const FAssetData& AssetData)
{
	FNotificationInfo Info(FText::Format(LOCTEXT("NothingToOpen", "{0} has no periodic, constant, ramp or left/right effect to open."), FText::FromName(AssetData.AssetName)));
	Info.ExpireDuration = 4.0f;
	FSlateNotificationManager::Get().AddNotification(Info);
}

TSharedRef<SWidget> SHapticEffectEditor::BuildAssetBar()
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			BuildOpenButton()
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			BuildSourceEntryPicker()
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			BuildSaveButton()
		];
}

TSharedRef<SWidget> SHapticEffectEditor::BuildOpenButton()
{
	return SNew(SComboButton)
		.ComboButtonStyle(FAppStyle::Get(), "SimpleComboButton")
		.ContentPadding(FMargin(6.0f, 2.0f))
		.IsFocusable(false)
		.ToolTipText(LOCTEXT("OpenTooltip", "The effect being edited. Open an existing effect Blueprint or effect group to edit it, or start a new effect."))
		.OnGetMenuContent_Lambda([this]() -> TSharedRef<SWidget>
		{
			FAssetPickerConfig AssetPickerConfig;
			AssetPickerConfig.Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
			AssetPickerConfig.Filter.ClassPaths.Add(UForceFeedbackEffectGroup::StaticClass()->GetClassPathName());
			AssetPickerConfig.Filter.bRecursiveClasses = true;
			AssetPickerConfig.InitialAssetViewType = EAssetViewType::List;
			AssetPickerConfig.bAllowNullSelection = false;
			AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateSP(this, &SHapticEffectEditor::OnSourceAssetPicked);
			AssetPickerConfig.OnShouldFilterAsset = FOnShouldFilterAsset::CreateLambda([](const FAssetData& AssetData)
			{
				// Effect groups are all listed: whether one holds an effect that can be opened isn't
				// known until it is loaded.
				return !CanEditAsset(AssetData);
			});

			const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

			FMenuBuilder MenuBuilder(true, nullptr);
			MenuBuilder.AddMenuEntry(
				LOCTEXT("NewEffect", "New Effect"),
				LOCTEXT("NewEffectTooltip", "Start a new effect from defaults. Any asset that was open is left as it was last saved."),
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateSP(this, &SHapticEffectEditor::NewEffect)));
			MenuBuilder.BeginSection("OpenExisting", LOCTEXT("OpenExistingSection", "Open Existing"));
			MenuBuilder.AddWidget(
				SNew(SBox)
				.WidthOverride(350.0f)
				.HeightOverride(400.0f)
				[
					ContentBrowserModule.Get().CreateAssetPicker(AssetPickerConfig)
				],
				FText::GetEmpty(),
				true);
			MenuBuilder.EndSection();
			return MenuBuilder.MakeWidget();
		})
		.ButtonContent()
		[
			SNew(STextBlock)
			.Text(this, &SHapticEffectEditor::GetSourceLabel)
			.TextStyle(FAppStyle::Get(), "NormalText")
		];
}

TSharedRef<SWidget> SHapticEffectEditor::BuildSourceEntryPicker()
{
	// Only an effect group with more than one effect that can be opened has anything to choose here.
	return SNew(SBox)
		.Padding(FMargin(4.0f, 0.0f, 0.0f, 0.0f))
		.Visibility_Lambda([this]
		{
			return SourceAsset.IsValid() && SourceEntryOptions.Num() > 1 ? EVisibility::Visible : EVisibility::Collapsed;
		})
		[
			SAssignNew(SourceEntryComboBox, SComboBox<TSharedPtr<int32>>)
			.OptionsSource(&SourceEntryOptions)
			.ToolTipText(LOCTEXT("SourceEntryTooltip", "Which of the effect group's entries is being edited. Changes that haven't been saved are dropped when switching."))
			.OnGenerateWidget_Lambda([this](const TSharedPtr<int32>& InItem) -> TSharedRef<SWidget>
			{
				return SNew(STextBlock)
					.Text(GetSourceEntryLabel(*InItem))
					.TextStyle(FAppStyle::Get(), "NormalText");
			})
			.OnSelectionChanged_Lambda([this](const TSharedPtr<int32>& NewSelection, const ESelectInfo::Type SelectInfo)
			{
				// Direct is this panel syncing the combo box to an entry it has already opened.
				if (!NewSelection.IsValid() || SelectInfo == ESelectInfo::Direct)
				{
					return;
				}

				if (PromptToSaveChanges())
				{
					OpenSource(SourceAsset.Get(), *NewSelection);
					return;
				}

				// Cancelled: put the combo box back on the entry that is still open.
				const TSharedPtr<int32>* OpenEntryOption = SourceEntryOptions.FindByPredicate([this](const TSharedPtr<int32>& Option)
				{
					return *Option == SourceEntryIndex;
				});
				if (OpenEntryOption && SourceEntryComboBox.IsValid())
				{
					SourceEntryComboBox->SetSelectedItem(*OpenEntryOption);
				}
			})
			[
				SNew(STextBlock)
				.Text_Lambda([this] { return GetSourceEntryLabel(SourceEntryIndex); })
				.TextStyle(FAppStyle::Get(), "NormalText")
			]
		];
}

TSharedRef<SWidget> SHapticEffectEditor::BuildSaveButton()
{
	return SNew(SButton)
		.ButtonStyle(FAppStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(6.0f, 2.0f))
		.IsFocusable(false)
		.ToolTipText_Lambda([this]
		{
			return SourceAsset.IsValid()
				       ? LOCTEXT("SaveTooltip", "Write these values back to the opened asset and save it (Ctrl+S).")
				       : LOCTEXT("SaveNewTooltip", "Save this effect as a new asset (Ctrl+S).");
		})
		.IsEnabled_Lambda([this] { return SourceAsset.IsValid() ? CanSaveToSource() : PreviewEffect.IsValid(); })
		.OnClicked(this, &SHapticEffectEditor::OnSaveClicked)
		[
			SNew(SImage)
			.Image(FAppStyle::GetBrush("Icons.Save"))
			.ColorAndOpacity(FSlateColor::UseForeground())
		];
}

void SHapticEffectEditor::PromptSaveAsNewAsset()
{
	// A new effect has no asset to save to yet, and can be saved in either of the two forms the plugin
	// consumes - which one is wanted depends on how the user means to use it, so it is asked for.
	const TOptional<EHapticEffectAssetType> AssetType = FHapticEffectAssetTypeDialog::Open();
	if (AssetType.IsSet())
	{
		SaveAsNewAsset(AssetType.GetValue());
	}
}

void SHapticEffectEditor::SaveAsNewAsset(const EHapticEffectAssetType AssetType)
{
	if (!PreviewEffect.IsValid())
	{
		return;
	}

	UClass* EffectClass = PreviewEffect->GetClass();
	const bool bExportBlueprint = AssetType == EHapticEffectAssetType::BlueprintClass;
	if (bExportBlueprint && !FKismetEditorUtilities::CanCreateBlueprintOfClass(EffectClass))
	{
		return;
	}

	FString DefaultAssetName;
	switch (EffectKind)
	{
		case EHapticPreviewEffectKind::Periodic:
			DefaultAssetName = TEXT("FFB_Periodic");
			break;
		case EHapticPreviewEffectKind::Constant:
			DefaultAssetName = TEXT("FFB_Constant");
			break;
		case EHapticPreviewEffectKind::Ramp:
			DefaultAssetName = TEXT("FFB_Ramp");
			break;
		case EHapticPreviewEffectKind::LeftRight:
			DefaultAssetName = TEXT("FFB_LeftRight");
			break;
	}

	FSaveAssetDialogConfig SaveAssetDialogConfig;
	SaveAssetDialogConfig.DialogTitleOverride = LOCTEXT("SaveAsDialogTitle", "Save Effect As");
	SaveAssetDialogConfig.DefaultPath = TEXT("/Game");
	SaveAssetDialogConfig.DefaultAssetName = DefaultAssetName;
	SaveAssetDialogConfig.AssetClassNames.Add((bExportBlueprint ? UBlueprint::StaticClass() : UForceFeedbackEffectGroup::StaticClass())->GetClassPathName());
	SaveAssetDialogConfig.ExistingAssetPolicy = ESaveAssetDialogExistingAssetPolicy::Disallow;

	const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	const FString SaveObjectPath = ContentBrowserModule.Get().CreateModalSaveAssetDialog(SaveAssetDialogConfig);
	if (SaveObjectPath.IsEmpty() || !PreviewEffect.IsValid())
	{
		return;
	}

	const FString PackageName = FPackageName::ObjectPathToPackageName(SaveObjectPath);
	const FString AssetName = FPackageName::GetLongPackageAssetName(PackageName);

	UPackage* Package = CreatePackage(*PackageName);
	UObject* ExportedAsset = nullptr;

	if (bExportBlueprint)
	{
		UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprint(EffectClass, Package, *AssetName, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass(), FName("HapticEffectEditor"));
		if (!Blueprint || !Blueprint->GeneratedClass)
		{
			return;
		}

		FHapticEffectEditorProperties::CopyAuthoredProperties(PreviewEffect.Get(), CastChecked<UForceFeedbackEffectBase>(Blueprint->GeneratedClass->GetDefaultObject()), EffectClass);
		ExportedAsset = Blueprint;
	}
	else
	{
		UForceFeedbackEffectGroup* EffectGroup = NewObject<UForceFeedbackEffectGroup>(Package, *AssetName, RF_Public | RF_Standalone | RF_Transactional);
		UForceFeedbackEffectBase* ExportedEffect = NewObject<UForceFeedbackEffectBase>(EffectGroup, EffectClass, NAME_None, RF_Transactional);
		FHapticEffectEditorProperties::CopyAuthoredProperties(PreviewEffect.Get(), ExportedEffect, EffectClass);

		FForceFeedbackEffectGroupEntry Entry;
		Entry.Effect = ExportedEffect;
		EffectGroup->Entries.Add(Entry);
		ExportedAsset = EffectGroup;
	}

	FAssetRegistryModule::AssetCreated(ExportedAsset);
	ExportedAsset->MarkPackageDirty();

	// The new asset is what is being edited from here on. The working copy is kept as it is rather
	// than reloaded from the asset - they hold the same values - so its undo history carries on.
	SourceAsset = ExportedAsset;
	SourceEntryIndex = bExportBlueprint ? INDEX_NONE : 0;
	SourceEntryOptions.Reset();
	if (!bExportBlueprint)
	{
		SourceEntryOptions.Add(MakeShared<int32>(0));
	}

	if (SourceEntryComboBox.IsValid())
	{
		SourceEntryComboBox->RefreshOptions();
	}

	FEditorFileUtils::PromptForCheckoutAndSave({Package}, false, false);
	ContentBrowserModule.Get().SyncBrowserToAssets(TArray<UObject*>{ExportedAsset});
}

FReply SHapticEffectEditor::OnSaveClicked()
{
	if (!SourceAsset.IsValid())
	{
		if (PreviewEffect.IsValid())
		{
			PromptSaveAsNewAsset();
		}

		return FReply::Handled();
	}

	UForceFeedbackEffectBase* SourceEffect = ResolveSourceEffect();
	if (!CanSaveToSource() || !SourceEffect)
	{
		return FReply::Handled();
	}

	// Not a transaction of its own: a save isn't something to undo, and undoing the edits that led up
	// to it still works on the working copy afterwards.
	SourceEffect->Modify();
	FHapticEffectEditorProperties::CopyAuthoredProperties(PreviewEffect.Get(), SourceEffect, PreviewEffect->GetClass());
	SourceAsset->MarkPackageDirty();

	// Then on to disk, as saving in an asset editor does. If that is cancelled or fails the values are
	// still in the asset, which is left marked as modified for the editor's own save prompts to catch.
	FEditorFileUtils::PromptForCheckoutAndSave({SourceAsset->GetPackage()}, false, false);

	return FReply::Handled();
}

void SHapticEffectEditor::NewEffect()
{
	if (!PromptToSaveChanges())
	{
		return;
	}

	CloseSource();
	RebuildPreviewEffect();
}

bool SHapticEffectEditor::PromptToSaveChanges()
{
	if (!CanSaveToSource())
	{
		return true;
	}

	const FText Message = FText::Format(LOCTEXT("SaveChangesPrompt", "Save changes to {0}?"), FText::FromString(SourceAsset->GetName()));
	switch (FMessageDialog::Open(EAppMsgType::YesNoCancel, Message))
	{
		case EAppReturnType::Yes:
			OnSaveClicked();
			return true;
		case EAppReturnType::No:
			return true;
		default:
			return false;
	}
}

void SHapticEffectEditor::OnSourceAssetPicked(const FAssetData& AssetData)
{
	FSlateApplication::Get().DismissAllMenus();

	if (!EditAsset(AssetData.GetAsset()))
	{
		NotifyNothingToOpen(AssetData);
	}
}

bool SHapticEffectEditor::CanEditAsset(const FAssetData& AssetData)
{
	const UClass* AssetClass = AssetData.GetClass();
	if (!AssetClass)
	{
		return false;
	}

	if (AssetClass->IsChildOf(UBlueprint::StaticClass()))
	{
		return FHapticEffectEditorProperties::IsSupportedEffectBlueprint(AssetData);
	}

	return AssetClass->IsChildOf(UForceFeedbackEffectGroup::StaticClass());
}

void SHapticEffectEditor::GetEditableEntries(const UForceFeedbackEffectGroup& EffectGroup, TArray<int32>& OutEntryIndices)
{
	for (int32 EntryIndex = 0; EntryIndex < EffectGroup.Entries.Num(); ++EntryIndex)
	{
		EHapticPreviewEffectKind Kind;
		const UForceFeedbackEffectBase* EntryEffect = EffectGroup.Entries[EntryIndex].Effect;
		if (EntryEffect && FHapticEffectEditorProperties::FindEffectKind(EntryEffect->GetClass(), Kind))
		{
			OutEntryIndices.Add(EntryIndex);
		}
	}
}

FText SHapticEffectEditor::GetEntryLabel(const UForceFeedbackEffectGroup& EffectGroup, const int32 EntryIndex)
{
	if (!EffectGroup.Entries.IsValidIndex(EntryIndex))
	{
		return FText::GetEmpty();
	}

	const FName EntryName = EffectGroup.Entries[EntryIndex].Name;
	return EntryName.IsNone() ? FText::Format(LOCTEXT("UnnamedEntry", "Entry {0}"), FText::AsNumber(EntryIndex)) : FText::FromName(EntryName);
}

bool SHapticEffectEditor::EditAsset(UObject* Asset, const int32 EntryIndex)
{
	if (const UForceFeedbackEffectGroup* EffectGroup = Cast<UForceFeedbackEffectGroup>(Asset))
	{
		TArray<int32> EditableEntries;
		GetEditableEntries(*EffectGroup, EditableEntries);
		if (EditableEntries.Num() == 0)
		{
			return false;
		}

		if (!PromptToSaveChanges())
		{
			return true;
		}

		const int32 EntryToOpen = EditableEntries.Contains(EntryIndex) ? EntryIndex : EditableEntries[0];

		SourceEntryOptions.Reset(EditableEntries.Num());
		TSharedPtr<int32> SelectedOption;
		for (const int32 EditableEntry : EditableEntries)
		{
			SourceEntryOptions.Add(MakeShared<int32>(EditableEntry));
			if (EditableEntry == EntryToOpen)
			{
				SelectedOption = SourceEntryOptions.Last();
			}
		}

		OpenSource(Asset, EntryToOpen);

		if (SourceEntryComboBox.IsValid())
		{
			SourceEntryComboBox->RefreshOptions();
			SourceEntryComboBox->SetSelectedItem(SelectedOption);
		}

		return true;
	}

	EHapticPreviewEffectKind Kind;
	const UBlueprint* Blueprint = Cast<UBlueprint>(Asset);
	if (!Blueprint || !FHapticEffectEditorProperties::FindEffectKind(Blueprint->GeneratedClass, Kind))
	{
		return false;
	}

	if (!PromptToSaveChanges())
	{
		return true;
	}

	SourceEntryOptions.Reset();
	OpenSource(Asset, INDEX_NONE);
	return true;
}

void SHapticEffectEditor::OpenSource(UObject* Asset, const int32 EntryIndex)
{
	SourceAsset = Asset;
	SourceEntryIndex = EntryIndex;

	const UForceFeedbackEffectBase* SourceEffect = ResolveSourceEffect();
	EHapticPreviewEffectKind Kind;
	if (!SourceEffect || !FHapticEffectEditorProperties::FindEffectKind(SourceEffect->GetClass(), Kind))
	{
		CloseSource();
		RebuildPreviewEffect();
		return;
	}

	EffectKind = Kind;
	RebuildPreviewEffect(SourceEffect);
}

void SHapticEffectEditor::CloseSource()
{
	SourceAsset.Reset();
	SourceEntryIndex = INDEX_NONE;
	SourceEntryOptions.Reset();

	if (SourceEntryComboBox.IsValid())
	{
		SourceEntryComboBox->RefreshOptions();
	}
}

// Looked up afresh on every use rather than cached: recompiling a Blueprint replaces its class
// default object, and a group's entries can be edited or removed in its own asset editor meanwhile.
UForceFeedbackEffectBase* SHapticEffectEditor::ResolveSourceEffect() const
{
	UObject* Asset = SourceAsset.Get();

	if (const UBlueprint* Blueprint = Cast<UBlueprint>(Asset))
	{
		return Blueprint->GeneratedClass ? Cast<UForceFeedbackEffectBase>(Blueprint->GeneratedClass->GetDefaultObject()) : nullptr;
	}

	if (const UForceFeedbackEffectGroup* EffectGroup = Cast<UForceFeedbackEffectGroup>(Asset))
	{
		return EffectGroup->Entries.IsValidIndex(SourceEntryIndex) ? EffectGroup->Entries[SourceEntryIndex].Effect : nullptr;
	}

	return nullptr;
}

bool SHapticEffectEditor::CanSaveToSource() const
{
	// Unsaved changes are worked out by comparing the working copy against the asset rather than
	// tracked with a flag, so undoing back to the saved values (or redoing forward to them) reads as
	// clean again without the undo history having to be followed.
	// The class check covers a group entry that has since been swapped for a different kind of effect.
	const UForceFeedbackEffectBase* SourceEffect = ResolveSourceEffect();
	return PreviewEffect.IsValid() && SourceEffect && SourceEffect->IsA(PreviewEffect->GetClass())
		&& !FHapticEffectEditorProperties::AuthoredPropertiesIdentical(PreviewEffect.Get(), SourceEffect, PreviewEffect->GetClass());
}

FText SHapticEffectEditor::GetSourceLabel() const
{
	if (const UObject* Asset = SourceAsset.Get())
	{
		// Same trailing asterisk the engine's own asset editors put on a tab with unsaved changes.
		return FText::FromString(Asset->GetName() + (CanSaveToSource() ? TEXT("*") : TEXT("")));
	}

	return LOCTEXT("SelectEffect", "Select Effect");
}

FText SHapticEffectEditor::GetTabLabel() const
{
	return SourceAsset.IsValid() ? GetSourceLabel() : LOCTEXT("TabLabel", "Haptic Effect Editor");
}

FText SHapticEffectEditor::GetSourceEntryLabel(const int32 EntryIndex) const
{
	const UForceFeedbackEffectGroup* EffectGroup = Cast<UForceFeedbackEffectGroup>(SourceAsset.Get());
	return EffectGroup ? GetEntryLabel(*EffectGroup, EntryIndex) : FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE
