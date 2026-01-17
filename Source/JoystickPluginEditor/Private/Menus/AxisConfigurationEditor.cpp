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
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "Styling/AppStyle.h"
#include "Styling/SlateTypes.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/DefaultStyleCache.h"
#include "Widgets/AxisBar.h"

void SAxisConfigurationEditor::Construct(const FArguments& InArgs)
{
	Key = InArgs._AxisKey;
	InstanceId = InArgs._InstanceId;
	KeyIndex = InArgs._AxisIndex;

	SelectedInputRange = -1;
	SelectedOutputRange = -1;

	// Get the default progress bar style and disable animation
	// Make sure we get a complete copy of the style with all brushes
	const FProgressBarStyle& DefaultProgressBarStyle = UE::Slate::Private::FDefaultStyleCache::GetRuntime().GetProgressBarStyle();
	ProgressBarStyle = DefaultProgressBarStyle;
	ProgressBarStyle.SetEnableFillAnimation(false);
	ProgressBarStyle.FillImage.TintColor = FLinearColor::White;

	// Initialize with default properties
	CurrentProperties = FJoystickInputDeviceAxisProperties();
	CurrentProperties.AxisIndex = KeyIndex;

	// Load current configuration if it exists
	LoadConfiguration();

	// Store original display name state for restart notification
	OriginalDisplayName = CurrentProperties.DisplayName;
	bOriginalOverrideDisplayName = CurrentProperties.OverrideDisplayName;

	ChildSlot
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
				.Text(FText::Format(FText::FromString("Axis Configuration - {0}"), Key.GetDisplayName()))
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
							.AxisIndex(KeyIndex)
							.DisplayName(Key.GetDisplayName())
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
								CurrentProperties.RemappingEnabled = CurrentProperties.RerangeInput || CurrentProperties.RerangeOutput || CurrentProperties.InvertInput;
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
								const bool bWasOverrideEnabled = CurrentProperties.OverrideDisplayName;
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
							SAssignNew(InputRangeDisabled, SCheckBox)
							.IsChecked(SelectedInputRange == -1 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
							.IsEnabled(SelectedInputRange != -1)
							.OnCheckStateChanged_Lambda([this](const ECheckBoxState NewState)
							{
								if (NewState == ECheckBoxState::Checked)
								{
									SelectedInputRange = -1;
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
							.Text(FText::FromString("Disabled"))
							.IsEnabled_Lambda([this]() { return SelectedInputRange == -1; })
							.TextStyle(FAppStyle::Get(), "NormalText")
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
						.FillWidth(0.2f)
						.VAlign(VAlign_Center)
						.Padding(0, 0, 8, 0)
						[
							SNew(STextBlock)
							.Text(FText::FromString("-1 to 0"))
							.IsEnabled_Lambda([this]() { return SelectedInputRange == 0; })
							.TextStyle(FAppStyle::Get(), "NormalText")
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
						.VAlign(VAlign_Center)
						[
							SNew(SProgressBar)
							                  .Percent(0.5) // 0% to 50% = 50% full
							                  .FillColorAndOpacity(FLinearColor(0.2f, 1.0f, 0.2f, 1.0f))
							                  .IsEnabled_Lambda([this]() { return SelectedInputRange == 0; })
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
						.FillWidth(0.2f)
						.VAlign(VAlign_Center)
						.Padding(0, 0, 8, 0)
						[
							SNew(STextBlock)
							.Text(FText::FromString("-1 to 1"))
							.IsEnabled_Lambda([this]() { return SelectedInputRange == 1; })
							.TextStyle(FAppStyle::Get(), "NormalText")
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
						.VAlign(VAlign_Center)
						[
							SNew(SProgressBar)
							                  .Percent(1.0f) // 0% to 100% = 100% full
							                  .FillColorAndOpacity(FLinearColor(0.2f, 1.0f, 0.2f, 1.0f))
							                  .IsEnabled_Lambda([this]() { return SelectedInputRange == 1; })
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
						.FillWidth(0.2f)
						.VAlign(VAlign_Center)
						.Padding(0, 0, 8, 0)
						[
							SNew(STextBlock)
							.Text(FText::FromString("0 to 1"))
							.TextStyle(FAppStyle::Get(), "NormalText")
							.IsEnabled_Lambda([this]() { return SelectedInputRange == 2; })
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
						.VAlign(VAlign_Center)
						[
							SNew(SProgressBar)
							                  .Percent(0.5f) // 0% to 100% = 100% full
							                  .BarFillType(EProgressBarFillType::RightToLeft)
							                  .FillColorAndOpacity(FLinearColor(0.2f, 1.0f, 0.2f, 1.0f))
							                  .IsEnabled_Lambda([this]() { return SelectedInputRange == 2; })
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
						.VAlign(VAlign_Top)
						.Padding(0, 0, 8, 0)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Top)
							.Padding(0, 0, 8, 0)
							[
								SAssignNew(InputRangeCustomRadio, SCheckBox)
								.IsChecked(SelectedInputRange == 3 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
								.IsEnabled(SelectedInputRange != 3)
								.OnCheckStateChanged_Lambda([this](const ECheckBoxState NewState)
								{
									if (NewState == ECheckBoxState::Checked)
									{
										SelectedInputRange = 3;
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
								.Text(FText::FromString("Custom"))
								.IsEnabled_Lambda([this]() { return SelectedInputRange == 3; })
								.TextStyle(FAppStyle::Get(), "NormalText")
							]
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.Padding(0, 0, 8, 0)
							.HAlign(HAlign_Left)
							[
								SNew(SVerticalBox)
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(0, 0, 0, 4)
								[
									SNew(SHorizontalBox)
									+ SHorizontalBox::Slot()
									.AutoWidth()
									.VAlign(VAlign_Center)
									.Padding(0, 0, 4, 0)
									[
										SNew(STextBlock)
										.Text(FText::FromString("Min:"))
										.IsEnabled_Lambda([this]() { return SelectedInputRange == 3; })
										.TextStyle(FAppStyle::Get(), "NormalText")
									]
									+ SHorizontalBox::Slot()
									.AutoWidth()
									[
										SAssignNew(CustomRangeMinInput, SNumericEntryBox<float>)
										.IsEnabled(SelectedInputRange == 3)
										.Value_Lambda([this]() { return CurrentProperties.InputRangeMin; })
										.OnValueChanged_Lambda([this](const float NewValue)
										{
											CurrentProperties.InputRangeMin = NewValue;
											if (SelectedInputRange == 3)
											{
												ApplyWorkingRange();
											}
										})
										.MinValue(-1.0f)
										.MaxValue(1.0f)
										.MinSliderValue(-1.0f)
										.MaxSliderValue(1.0f)
										.AllowSpin(true)
										.SliderExponent(1.0f)
										.Delta(0.01f)
										.MinDesiredValueWidth(80.0f)
									]
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								.HAlign(HAlign_Left)
								[
									SAssignNew(SetMinButton, SButton)
									.Text(FText::FromString("Set"))
									.IsEnabled(SelectedInputRange == 3)
									.OnClicked(this, &SAxisConfigurationEditor::OnSetMinButtonClicked)
								]
							]
							+ SHorizontalBox::Slot()
							.FillWidth(1.0f)
							[
								SNullWidget::NullWidget
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.HAlign(HAlign_Right)
							[
								SNew(SVerticalBox)
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(0, 0, 0, 4)
								[
									SNew(SHorizontalBox)
									+ SHorizontalBox::Slot()
									.AutoWidth()
									.VAlign(VAlign_Center)
									.Padding(0, 0, 4, 0)
									[
										SNew(STextBlock)
										.Text(FText::FromString("Max:"))
										.IsEnabled_Lambda([this]() { return SelectedInputRange == 3; })
										.TextStyle(FAppStyle::Get(), "NormalText")
									]
									+ SHorizontalBox::Slot()
									.AutoWidth()
									[
										SAssignNew(CustomRangeMaxInput, SNumericEntryBox<float>)
										.IsEnabled(SelectedInputRange == 3)
										.Value_Lambda([this]() { return CurrentProperties.InputRangeMax; })
										.OnValueChanged_Lambda([this](const float NewValue)
										{
											CurrentProperties.InputRangeMax = NewValue;
											if (SelectedInputRange == 3)
											{
												ApplyWorkingRange();
											}
										})
										.MinValue(-1.0f)
										.MaxValue(1.0f)
										.MinSliderValue(-1.0f)
										.MaxSliderValue(1.0f)
										.AllowSpin(true)
										.SliderExponent(1.0f)
										.Delta(0.01f)
										.MinDesiredValueWidth(80.0f)
									]
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								.HAlign(HAlign_Right)
								[
									SAssignNew(SetMaxButton, SButton)
									.Text(FText::FromString("Set"))
									.IsEnabled(SelectedInputRange == 3)
									.OnClicked(this, &SAxisConfigurationEditor::OnSetMaxButtonClicked)
								]
							]
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
							SAssignNew(OutputRangeDisabled, SCheckBox)
							.IsChecked(SelectedOutputRange == -1 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
							.IsEnabled(SelectedOutputRange != -1)
							.OnCheckStateChanged_Lambda([this](const ECheckBoxState NewState)
							{
								if (NewState == ECheckBoxState::Checked)
								{
									SelectedOutputRange = -1;
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
							.Text(FText::FromString("Disabled"))
							.IsEnabled_Lambda([this]() { return SelectedOutputRange == -1; })
							.TextStyle(FAppStyle::Get(), "NormalText")
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
						.FillWidth(0.2f)
						.VAlign(VAlign_Center)
						.Padding(0, 0, 8, 0)
						[
							SNew(STextBlock)
							.Text(FText::FromString("0 to 1"))
							.IsEnabled_Lambda([this]() { return SelectedOutputRange == 0; })
							.TextStyle(FAppStyle::Get(), "NormalText")
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
						.VAlign(VAlign_Center)
						[
							SNew(SProgressBar)
							                  .Percent(0.5f) // 50% full (0% to 50% range)
							                  .BarFillType(EProgressBarFillType::RightToLeft)
							                  .FillColorAndOpacity(FLinearColor(0.2f, 1.0f, 0.2f, 1.0f))
							                  .IsEnabled_Lambda([this]() { return SelectedOutputRange == 0; })
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
						.FillWidth(0.2f)
						.VAlign(VAlign_Center)
						.Padding(0, 0, 8, 0)
						[
							SNew(STextBlock)
							.Text(FText::FromString("-1 to 1"))
							.IsEnabled_Lambda([this]() { return SelectedOutputRange == 1; })
							.TextStyle(FAppStyle::Get(), "NormalText")
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
						.VAlign(VAlign_Center)
						[
							SNew(SProgressBar)
							                  .Percent(1.0f) // 100% full
							                  .FillColorAndOpacity(FLinearColor(0.2f, 1.0f, 0.2f, 1.0f))
							                  .IsEnabled_Lambda([this]() { return SelectedOutputRange == 1; })
							                  .Style(&ProgressBarStyle)
						]
					]
				]
			]

			// Action Buttons
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				MakeActionButtons()
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

	if (!JoystickState.Axes.IsValidIndex(KeyIndex))
	{
		return;
	}

	const float CurrentValue = JoystickState.Axes[KeyIndex].GetMockValue(CurrentProperties);
	CurrentValueText->SetText(FText::FromString(FString::Printf(TEXT("Value: %.3f"), CurrentValue)));
	InputVisualiser->SetValue(CurrentValue);
}

void SAxisConfigurationEditor::LoadConfiguration()
{
	UJoystickProfileManager* ProfileManager = GetMutableDefault<UJoystickProfileManager>();
	if (!IsValid(ProfileManager))
	{
		return;
	}

	FJoystickInputDeviceAxisProperties LoadedProperties;
	if (ProfileManager->GetAxisConfiguration(Key, LoadedProperties))
	{
		CurrentProperties = LoadedProperties;
		CurrentProperties.AxisIndex = KeyIndex;

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
			else
			{
				SelectedInputRange = 3; // custom
			}
		}
		else
		{
			SelectedInputRange = -1;
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
		else
		{
			SelectedOutputRange = -1;
		}

		UpdateInputRangeButtons();
		UpdateOutputRangeButtons();
	}
}

void SAxisConfigurationEditor::SaveConfiguration() const
{
	UJoystickProfileManager* ProfileManager = GetMutableDefault<UJoystickProfileManager>();
	if (!IsValid(ProfileManager))
	{
		return;
	}

	if (CurrentProperties.OverrideDisplayName != bOriginalOverrideDisplayName || CurrentProperties.DisplayName != OriginalDisplayName)
	{
		ShowRestartRequiredNotification();
	}

	ProfileManager->UpdateAxisConfiguration(Key, CurrentProperties);
}

void SAxisConfigurationEditor::ApplyWorkingRange()
{
	switch (SelectedInputRange)
	{
	case 0: // -1 to 0
		CurrentProperties.RerangeInput = true;
		CurrentProperties.InputRangeMin = -1.0f;
		CurrentProperties.InputRangeMax = 0.0f;
		break;
	case 1: // -1 to 1
		CurrentProperties.RerangeInput = true;
		CurrentProperties.InputRangeMin = -1.0f;
		CurrentProperties.InputRangeMax = 1.0f;
		break;
	case 2: // 0 to 1
		CurrentProperties.RerangeInput = true;
		CurrentProperties.InputRangeMin = 0.0f;
		CurrentProperties.InputRangeMax = 1.0f;
		break;
	case 3: // custom
		CurrentProperties.RerangeInput = true;
		// Values are already set by the numeric entry boxes
		break;
	case -1:
	default:
		CurrentProperties.RerangeInput = false;
		break;
	}

	CurrentProperties.RemappingEnabled = CurrentProperties.RerangeInput || CurrentProperties.RerangeOutput || CurrentProperties.InvertInput;

	UpdateInputRangeButtons();
}

void SAxisConfigurationEditor::ApplyOutputRange()
{
	switch (SelectedOutputRange)
	{
	case 0:
		CurrentProperties.RerangeOutput = true;
		CurrentProperties.OutputRangeMin = 0.0f;
		CurrentProperties.OutputRangeMax = 1.0f;
		break;
	case 1:
		CurrentProperties.RerangeOutput = true;
		CurrentProperties.OutputRangeMin = -1.0f;
		CurrentProperties.OutputRangeMax = 1.0f;
		break;
	case -1:
	default:
		CurrentProperties.RerangeOutput = false;
		break;
	}

	CurrentProperties.RemappingEnabled = CurrentProperties.RerangeInput || CurrentProperties.RerangeOutput || CurrentProperties.InvertInput;

	UpdateOutputRangeButtons();
}

void SAxisConfigurationEditor::UpdateInputRangeButtons() const
{
	if (InputRangeDisabled.IsValid())
	{
		InputRangeDisabled->SetIsChecked(SelectedInputRange == -1 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		InputRangeDisabled->SetEnabled(SelectedInputRange != -1);
	}
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
	if (InputRangeCustomRadio.IsValid())
	{
		InputRangeCustomRadio->SetIsChecked(SelectedInputRange == 3 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		InputRangeCustomRadio->SetEnabled(SelectedInputRange != 3);
	}
	if (CustomRangeMinInput.IsValid())
	{
		CustomRangeMinInput->SetEnabled(SelectedInputRange == 3);
	}
	if (CustomRangeMaxInput.IsValid())
	{
		CustomRangeMaxInput->SetEnabled(SelectedInputRange == 3);
	}
	if (SetMinButton.IsValid())
	{
		SetMinButton->SetEnabled(SelectedInputRange == 3);
	}
	if (SetMaxButton.IsValid())
	{
		SetMaxButton->SetEnabled(SelectedInputRange == 3);
	}
}


void SAxisConfigurationEditor::UpdateOutputRangeButtons() const
{
	if (OutputRangeDisabled.IsValid())
	{
		OutputRangeDisabled->SetIsChecked(SelectedOutputRange == -1 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		OutputRangeDisabled->SetEnabled(SelectedOutputRange != -1);
	}
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

float SAxisConfigurationEditor::GetCurrentAxisValue() const
{
	if (!GEngine)
	{
		return 0.0f;
	}

	const UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return 0.0f;
	}

	FJoystickDeviceState JoystickState;
	const bool Result = JoystickSubsystem->GetJoystickState(InstanceId, JoystickState);
	if (Result == false)
	{
		return 0.0f;
	}

	if (!JoystickState.Axes.IsValidIndex(KeyIndex))
	{
		return 0.0f;
	}

	// Get the raw value (not remapped)
	return JoystickState.Axes[KeyIndex].Value;
}

FReply SAxisConfigurationEditor::OnSetMinButtonClicked()
{
	const float CurrentValue = GetCurrentAxisValue();
	CurrentProperties.InputRangeMin = CurrentValue;

	if (SelectedInputRange == 3)
	{
		ApplyWorkingRange();
	}

	return FReply::Handled();
}

FReply SAxisConfigurationEditor::OnSetMaxButtonClicked()
{
	const float CurrentValue = GetCurrentAxisValue();
	CurrentProperties.InputRangeMax = CurrentValue;

	if (SelectedInputRange == 3)
	{
		ApplyWorkingRange();
	}

	return FReply::Handled();
}
