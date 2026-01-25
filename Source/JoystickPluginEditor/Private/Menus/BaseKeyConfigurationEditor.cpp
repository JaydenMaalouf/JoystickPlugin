// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Menus/BaseKeyConfigurationEditor.h"

#include "ISettingsEditorModule.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"

void SBaseKeyConfigurationEditor::Construct(const FArguments& InArgs)
{
	Key = InArgs._Key;
	InstanceId = InArgs._InstanceId;
	KeyIndex = InArgs._Index;
}

SBaseKeyConfigurationEditor::~SBaseKeyConfigurationEditor()
{
}

void SBaseKeyConfigurationEditor::LoadConfiguration()
{
}

void SBaseKeyConfigurationEditor::SaveConfiguration() const
{
}

void SBaseKeyConfigurationEditor::ShowRestartRequiredNotification() const
{
	if (ISettingsEditorModule* SettingsEditorModule = FModuleManager::GetModulePtr<ISettingsEditorModule>("SettingsEditor"))
	{
		SettingsEditorModule->OnApplicationRestartRequired();
	}
}

FReply SBaseKeyConfigurationEditor::OnSaveClicked()
{
	SaveConfiguration();

	// Close the window
	const TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
	if (ParentWindow.IsValid())
	{
		ParentWindow->RequestDestroyWindow();
	}

	return FReply::Handled();
}

FReply SBaseKeyConfigurationEditor::OnCancelClicked()
{
	// Close the window without saving
	const TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
	if (ParentWindow.IsValid())
	{
		ParentWindow->RequestDestroyWindow();
	}

	return FReply::Handled();
}

TSharedRef<SWidget> SBaseKeyConfigurationEditor::MakeActionButtons()
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SNew(SButton)
			.Text(FText::FromString("Save"))
			.OnClicked(this, &SBaseKeyConfigurationEditor::OnSaveClicked)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8, 0, 0, 0)
		[
			SNew(SButton)
			.Text(FText::FromString("Cancel"))
			.OnClicked(this, &SBaseKeyConfigurationEditor::OnCancelClicked)
		];
}
