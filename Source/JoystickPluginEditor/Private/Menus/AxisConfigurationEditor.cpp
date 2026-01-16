// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Menus/AxisConfigurationEditor.h"

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
#include "Widgets/Notifications/SProgressBar.h"
#include "Styling/AppStyle.h"
#include "Styling/SlateTypes.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/AxisBar.h"

void SAxisConfigurationEditor::Construct(const FArguments& InArgs)
{
	AxisKey = InArgs._AxisKey;
	InstanceId = InArgs._InstanceId;
	AxisIndex = InArgs._AxisIndex;
	SelectedInputRange = -1;
	SelectedOutputRange = -1;

	// Get the default progress bar style and disable animation
	// Make sure we get a complete copy of the style with all brushes
	const FProgressBarStyle& DefaultProgressBarStyle = FAppStyle::Get().GetWidgetStyle<FProgressBarStyle>("ProgressBar");
	ProgressBarStyle = DefaultProgressBarStyle;
	ProgressBarStyle.SetEnableFillAnimation(false);

	// Initialize with default properties
	CurrentProperties = FJoystickInputDeviceAxisProperties();
	CurrentProperties.AxisIndex = AxisIndex;

	// Load current configuration if it exists
	LoadCurrentConfiguration();

	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(600.0f)
		.HeightOverride(700.0f)
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
					.Text(FText::Format(FText::FromString("Axis Configuration - {0}"), AxisKey.GetDisplayName()))
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
							SAssignNew(CurrentValueText, STextBlock)
							.Text(FText::FromString("Value: 0.000"))
							.TextStyle(FAppStyle::Get(), "NormalText")
							.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SBox)
							.HAlign(HAlign_Center)
							[
								SAssignNew(InputVisualiser, SAxisBar)
								.AxisIndex(AxisIndex)
								.DisplayName(AxisKey.GetDisplayName())
								.Value(0.0f)
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
								.Text(FText::FromString("Invert Input:"))
								.TextStyle(FAppStyle::Get(), "NormalText")
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								SAssignNew(InvertInputCheckBox, SCheckBox)
								.IsChecked_Lambda([this]() { return CurrentProperties.InvertInput ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
								.OnCheckStateChanged_Lambda([this](const ECheckBoxState NewState)
								{
									CurrentProperties.InvertInput = (NewState == ECheckBoxState::Checked);
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

				// Working Range Section
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
							.Text(FText::FromString("Working Range:"))
							.TextStyle(FAppStyle::Get(), "DetailsView.CategoryTextStyle")
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0, 0, 0, 4)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							.Padding(0, 0, 8, 0)
							[
								SAssignNew(InputRangeNeg1To0Radio, SCheckBox)
								.IsChecked(SelectedInputRange == 0 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
								.IsEnabled(SelectedInputRange != 0)
								.OnCheckStateChanged_Lambda([this](const ECheckBoxState NewState)
								{
									if (NewState == ECheckBoxState::Checked)
									{
										SelectedInputRange = 0;
										ApplyWorkingRange();
									}
								})
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							.Padding(0, 0, 8, 0)
							[
								SNew(STextBlock)
								.Text(FText::FromString("-1 to 0"))
								.TextStyle(FAppStyle::Get(), "NormalText")
							]
							+ SHorizontalBox::Slot()
							.FillWidth(1.0f)
							.VAlign(VAlign_Center)
							[
								SNew(SProgressBar)
								                  .Percent(0.5) // 0% to 50% = 50% full
								                  .FillColorAndOpacity(FLinearColor(0.2f, 0.6f, 0.9f, 1.0f))
								                  .Style(&ProgressBarStyle)
							]
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0, 0, 0, 4)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							.Padding(0, 0, 8, 0)
							[
								SAssignNew(InputRangeNeg1To1Radio, SCheckBox)
								.IsChecked(SelectedInputRange == 1 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
								.IsEnabled(SelectedInputRange != 1)
								.OnCheckStateChanged_Lambda([this](const ECheckBoxState NewState)
								{
									if (NewState == ECheckBoxState::Checked)
									{
										SelectedInputRange = 1;
										ApplyWorkingRange();
									}
								})
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							.Padding(0, 0, 8, 0)
							[
								SNew(STextBlock)
								.Text(FText::FromString("-1 to 1"))
								.TextStyle(FAppStyle::Get(), "NormalText")
							]
							+ SHorizontalBox::Slot()
							.FillWidth(1.0f)
							.VAlign(VAlign_Center)
							[
								SNew(SProgressBar)
								                  .Percent(1.0f) // 0% to 100% = 100% full
								                  .FillColorAndOpacity(FLinearColor(0.2f, 0.6f, 0.9f, 1.0f))
								                  .Style(&ProgressBarStyle)
							]
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0, 0, 0, 4)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							.Padding(0, 0, 8, 0)
							[
								SAssignNew(InputRange0To1Radio, SCheckBox)
								.IsChecked(SelectedInputRange == 2 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
								.IsEnabled(SelectedInputRange != 2)
								.OnCheckStateChanged_Lambda([this](const ECheckBoxState NewState)
								{
									if (NewState == ECheckBoxState::Checked)
									{
										SelectedInputRange = 2;
										ApplyWorkingRange();
									}
								})
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							.Padding(0, 0, 8, 0)
							[
								SNew(STextBlock)
								.Text(FText::FromString("0 to 1"))
								.TextStyle(FAppStyle::Get(), "NormalText")
							]
							+ SHorizontalBox::Slot()
							.FillWidth(1.0f)
							.VAlign(VAlign_Center)
							[
								SNew(SProgressBar)
								                  .Percent(0.5f) // 0% to 100% = 100% full
								                  .BarFillType(EProgressBarFillType::RightToLeft)
								                  .FillColorAndOpacity(FLinearColor(0.2f, 0.6f, 0.9f, 1.0f))
								                  .Style(&ProgressBarStyle)
							]
						]
					]
				]

				// Output Range Section
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
							.Text(FText::FromString("Output Range:"))
							.TextStyle(FAppStyle::Get(), "DetailsView.CategoryTextStyle")
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0, 0, 0, 4)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							.Padding(0, 0, 8, 0)
							[
								SAssignNew(OutputRange0To1Radio, SCheckBox)
								.IsChecked(SelectedOutputRange == 0 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
								.IsEnabled(SelectedOutputRange != 0)
								.OnCheckStateChanged_Lambda([this](const ECheckBoxState NewState)
								{
									if (NewState == ECheckBoxState::Checked)
									{
										SelectedOutputRange = 0;
										ApplyOutputRange();
									}
								})
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							.Padding(0, 0, 8, 0)
							[
								SNew(STextBlock)
								.Text(FText::FromString("0 to 1"))
								.TextStyle(FAppStyle::Get(), "NormalText")
							]
							+ SHorizontalBox::Slot()
							.FillWidth(1.0f)
							.VAlign(VAlign_Center)
							[
								SNew(SProgressBar)
								                  .Percent(0.5f) // 50% full (0% to 50% range)
								                  .BarFillType(EProgressBarFillType::RightToLeft)
								                  .FillColorAndOpacity(FLinearColor(0.2f, 0.6f, 0.9f, 1.0f))
								                  .Style(&ProgressBarStyle)
							]
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0, 0, 0, 4)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							.Padding(0, 0, 8, 0)
							[
								SAssignNew(OutputRangeNeg1To1Radio, SCheckBox)
								.IsChecked(SelectedOutputRange == 1 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
								.IsEnabled(SelectedOutputRange != 1)
								.OnCheckStateChanged_Lambda([this](const ECheckBoxState NewState)
								{
									if (NewState == ECheckBoxState::Checked)
									{
										SelectedOutputRange = 1;
										ApplyOutputRange();
									}
								})
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							.Padding(0, 0, 8, 0)
							[
								SNew(STextBlock)
								.Text(FText::FromString("-1 to 1"))
								.TextStyle(FAppStyle::Get(), "NormalText")
							]
							+ SHorizontalBox::Slot()
							.FillWidth(1.0f)
							.VAlign(VAlign_Center)
							[
								SNew(SProgressBar)
								                  .Percent(1.0f) // 100% full
								                  .FillColorAndOpacity(FLinearColor(0.2f, 0.6f, 0.9f, 1.0f))
								                  .Style(&ProgressBarStyle)
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
						.OnClicked(this, &SAxisConfigurationEditor::OnSaveClicked)
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(8, 0, 0, 0)
					[
						SNew(SButton)
						.Text(FText::FromString("Cancel"))
						.OnClicked(this, &SAxisConfigurationEditor::OnCancelClicked)
					]
				]
			]
		]
	];
}

SAxisConfigurationEditor::~SAxisConfigurationEditor()
{
}

void SAxisConfigurationEditor::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (!CurrentValueText.IsValid() || !InputVisualiser.IsValid())
	{
		return;
	}

	// Update the visualizer with current axis value
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

	if (!JoystickState.Axes.IsValidIndex(AxisIndex))
	{
		return;
	}

	const float CurrentValue = JoystickState.Axes[AxisIndex].GetMockValue(CurrentProperties);
	CurrentValueText->SetText(FText::FromString(FString::Printf(TEXT("Value: %.3f"), CurrentValue)));
	InputVisualiser->SetValue(CurrentValue);
}

void SAxisConfigurationEditor::LoadCurrentConfiguration()
{
	UJoystickProfileManager* ProfileManager = GetMutableDefault<UJoystickProfileManager>();
	if (!IsValid(ProfileManager))
	{
		return;
	}

	FJoystickInputDeviceAxisProperties LoadedProperties;
	if (ProfileManager->GetAxisConfiguration(AxisKey, LoadedProperties))
	{
		CurrentProperties = LoadedProperties;
		CurrentProperties.AxisIndex = AxisIndex;

		// Determine selected input range based on loaded properties
		if (CurrentProperties.RerangeInput)
		{
			if (CurrentProperties.InputRangeMin == -1.0f && CurrentProperties.InputRangeMax == 0.0f)
			{
				SelectedInputRange = 0; // -1 to 0
			}
			else if (CurrentProperties.InputRangeMin == -1.0f && CurrentProperties.InputRangeMax == 1.0f)
			{
				SelectedInputRange = 1; // -1 to 1
			}
			else if (CurrentProperties.InputRangeMin == 0.0f && CurrentProperties.InputRangeMax == 1.0f)
			{
				SelectedInputRange = 2; // 0 to 1
			}
		}

		// Determine output range
		if (CurrentProperties.RerangeOutput)
		{
			if (CurrentProperties.OutputRangeMin == 0.0f && CurrentProperties.OutputRangeMax == 1.0f)
			{
				SelectedOutputRange = 0;
			}
			else if (CurrentProperties.OutputRangeMin == -1.0f && CurrentProperties.OutputRangeMax == 1.0f)
			{
				SelectedOutputRange = 1;
			}
		}

		UpdateInputRangeButtons();
		UpdateOutputRangeButtons();
	}
}

void SAxisConfigurationEditor::ApplyWorkingRange()
{
	CurrentProperties.RemappingEnabled = true;
	CurrentProperties.RerangeInput = true;

	switch (SelectedInputRange)
	{
	case 0: // -1 to 0
		CurrentProperties.InputRangeMin = -1.0f;
		CurrentProperties.InputRangeMax = 0.0f;
		break;
	case 1: // -1 to 1
		CurrentProperties.InputRangeMin = -1.0f;
		CurrentProperties.InputRangeMax = 1.0f;
		break;
	case 2: // 0 to 1
		CurrentProperties.InputRangeMin = 0.0f;
		CurrentProperties.InputRangeMax = 1.0f;
		break;
	default:
		break;
	}

	UpdateInputRangeButtons();
}

void SAxisConfigurationEditor::ApplyOutputRange()
{
	CurrentProperties.RemappingEnabled = true;
	CurrentProperties.RerangeOutput = true;

	switch (SelectedOutputRange)
	{
	case 0:
		CurrentProperties.OutputRangeMin = 0.0f;
		CurrentProperties.OutputRangeMax = 1.0f;
		break;
	case 1:
		CurrentProperties.OutputRangeMin = -1.0f;
		CurrentProperties.OutputRangeMax = 1.0f;
		break;
	default:
		break;
	}

	UpdateOutputRangeButtons();
}

void SAxisConfigurationEditor::SaveConfiguration() const
{
	UJoystickProfileManager* ProfileManager = GetMutableDefault<UJoystickProfileManager>();
	if (!IsValid(ProfileManager))
	{
		return;
	}

	ProfileManager->UpdateAxisConfiguration(AxisKey, CurrentProperties);
}

void SAxisConfigurationEditor::UpdateInputRangeButtons() const
{
	if (InputRangeNeg1To0Radio.IsValid())
	{
		InputRangeNeg1To0Radio->SetIsChecked(SelectedInputRange == 0 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		InputRangeNeg1To0Radio->SetEnabled(SelectedInputRange != 0);
	}
	if (InputRangeNeg1To1Radio.IsValid())
	{
		InputRangeNeg1To1Radio->SetIsChecked(SelectedInputRange == 1 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		InputRangeNeg1To1Radio->SetEnabled(SelectedInputRange != 1);
	}
	if (InputRange0To1Radio.IsValid())
	{
		InputRange0To1Radio->SetIsChecked(SelectedInputRange == 2 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		InputRange0To1Radio->SetEnabled(SelectedInputRange != 2);
	}
}


void SAxisConfigurationEditor::UpdateOutputRangeButtons() const
{
	if (OutputRange0To1Radio.IsValid())
	{
		OutputRange0To1Radio->SetIsChecked(SelectedOutputRange == 0 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		OutputRange0To1Radio->SetEnabled(SelectedOutputRange != 0);
	}
	if (OutputRangeNeg1To1Radio.IsValid())
	{
		OutputRangeNeg1To1Radio->SetIsChecked(SelectedOutputRange == 1 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		OutputRangeNeg1To1Radio->SetEnabled(SelectedOutputRange != 1);
	}
}

FReply SAxisConfigurationEditor::OnSaveClicked()
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

FReply SAxisConfigurationEditor::OnCancelClicked()
{
	// Close the window without saving
	const TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
	if (ParentWindow.IsValid())
	{
		ParentWindow->RequestDestroyWindow();
	}

	return FReply::Handled();
}
