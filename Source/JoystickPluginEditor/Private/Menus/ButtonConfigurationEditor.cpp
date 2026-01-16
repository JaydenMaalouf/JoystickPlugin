// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Menus/ButtonConfigurationEditor.h"

#include "Engine/Engine.h"
#include "Managers/JoystickProfileManager.h"
#include "JoystickSubsystem.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Styling/AppStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/ButtonBox.h"

void SButtonConfigurationEditor::Construct(const FArguments& InArgs)
{
	ButtonKey = InArgs._ButtonKey;
	InstanceId = InArgs._InstanceId;
	ButtonIndex = InArgs._ButtonIndex;

	// Initialize with default properties
	CurrentProperties = FJoystickInputDeviceButtonProperties();
	CurrentProperties.ButtonIndex = ButtonIndex;

	// Load current configuration if it exists
	LoadCurrentConfiguration();

	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(600.0f)
		.HeightOverride(500.0f)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			.Padding(16)
			[
				SNew(SVerticalBox)

				// Title
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 16)
				[
					SNew(STextBlock)
					.Text(FText::Format(FText::FromString("Button Configuration - {0}"), ButtonKey.GetDisplayName()))
					.TextStyle(FAppStyle::Get(), "DetailsView.CategoryTextStyle")
					.Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
				]

				// Current Input Visualizer Section
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 16)
				[
					SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
					.Padding(12)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0, 0, 0, 8)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Current Input"))
							.TextStyle(FAppStyle::Get(), "DetailsView.CategoryTextStyle")
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0, 0, 0, 8)
						[
							SNew(SBox)
							.HAlign(HAlign_Center)
							[
								SAssignNew(InputVisualiser, SButtonBox)
								.ButtonIndex(ButtonIndex)
								.DisplayName(ButtonKey.GetDisplayName())
								.Value(false)
							]
						]
					]
				]

				// Configuration Section
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 16)
				[
					SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
					.Padding(12)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0, 0, 0, 8)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Configuration:"))
							.TextStyle(FAppStyle::Get(), "DetailsView.CategoryTextStyle")
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0, 4)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							.Padding(0, 0, 8, 0)
							[
								SNew(STextBlock)
								.Text(FText::FromString("Invert Output:"))
								.TextStyle(FAppStyle::Get(), "NormalText")
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								SAssignNew(InvertOutputCheckBox, SCheckBox)
								.IsChecked_Lambda([this]() { return CurrentProperties.InvertOutput ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
								.OnCheckStateChanged_Lambda([this](const ECheckBoxState NewState)
								{
									CurrentProperties.InvertOutput = (NewState == ECheckBoxState::Checked);
								})
							]
						]

						// Override Display Name
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0, 4)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							.Padding(0, 0, 8, 0)
							[
								SNew(STextBlock)
								.Text(FText::FromString("Override Display Name:"))
								.TextStyle(FAppStyle::Get(), "NormalText")
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								SAssignNew(OverrideDisplayNameCheckBox, SCheckBox)
								.IsChecked_Lambda([this]() { return CurrentProperties.OverrideDisplayName ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
								.OnCheckStateChanged_Lambda([this](const ECheckBoxState NewState)
								{
									CurrentProperties.OverrideDisplayName = (NewState == ECheckBoxState::Checked);
									if (DisplayNameInputContainer.IsValid())
									{
										DisplayNameInputContainer->SetVisibility(CurrentProperties.OverrideDisplayName ? EVisibility::Visible : EVisibility::Collapsed);
									}
								})
							]
						]

						// Display Name Input Field
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0, 4)
						[
							SAssignNew(DisplayNameInputContainer, SHorizontalBox)
							.Visibility(CurrentProperties.OverrideDisplayName ? EVisibility::Visible : EVisibility::Collapsed)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							.Padding(0, 0, 8, 0)
							[
								SNew(STextBlock)
								.Text(FText::FromString("Display Name:"))
								.TextStyle(FAppStyle::Get(), "NormalText")
							]
							+ SHorizontalBox::Slot()
							.FillWidth(1.0f)
							[
								SAssignNew(DisplayNameTextBox, SEditableTextBox)
								.Text(FText::FromString(CurrentProperties.DisplayName))
								.OnTextChanged_Lambda([this](const FText& NewText)
								{
									CurrentProperties.DisplayName = NewText.ToString();
								})
							]
						]
					]
				]

				// Action Buttons
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(SButton)
						.Text(FText::FromString("Save"))
						.OnClicked(this, &SButtonConfigurationEditor::OnSaveClicked)
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(8, 0, 0, 0)
					[
						SNew(SButton)
						.Text(FText::FromString("Cancel"))
						.OnClicked(this, &SButtonConfigurationEditor::OnCancelClicked)
					]
				]
			]
		]
	];
}

SButtonConfigurationEditor::~SButtonConfigurationEditor()
{
}

void SButtonConfigurationEditor::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (!InputVisualiser.IsValid())
	{
		return;
	}

	// Update the visualizer with current button value
	if (!GEngine)
	{
		return;
	}

	const UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	FJoystickDeviceState JoystickState;
	const bool Result = JoystickSubsystem->GetJoystickState(InstanceId, JoystickState);
	if (Result == false)
	{
		return;
	}

	if (!JoystickState.Buttons.IsValidIndex(ButtonIndex))
	{
		return;
	}

	const bool OutputValue = JoystickState.Buttons[ButtonIndex].GetMockValue(CurrentProperties);
	InputVisualiser->SetValue(OutputValue);
}

void SButtonConfigurationEditor::LoadCurrentConfiguration()
{
	UJoystickProfileManager* ProfileManager = GetMutableDefault<UJoystickProfileManager>();
	if (!IsValid(ProfileManager))
	{
		return;
	}

	FJoystickInputDeviceButtonProperties LoadedProperties;
	if (ProfileManager->GetButtonConfiguration(ButtonKey, LoadedProperties))
	{
		CurrentProperties = LoadedProperties;
		CurrentProperties.ButtonIndex = ButtonIndex;
	}
}

void SButtonConfigurationEditor::SaveConfiguration() const
{
	UJoystickProfileManager* ProfileManager = GetMutableDefault<UJoystickProfileManager>();
	if (!IsValid(ProfileManager))
	{
		return;
	}

	ProfileManager->UpdateButtonConfiguration(ButtonKey, CurrentProperties);
}

FReply SButtonConfigurationEditor::OnSaveClicked()
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

FReply SButtonConfigurationEditor::OnCancelClicked()
{
	// Close the window without saving
	const TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
	if (ParentWindow.IsValid())
	{
		ParentWindow->RequestDestroyWindow();
	}

	return FReply::Handled();
}
