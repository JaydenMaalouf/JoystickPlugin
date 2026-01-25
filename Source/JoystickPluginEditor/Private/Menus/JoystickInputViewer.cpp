// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Menus/JoystickInputViewer.h"

#include "JoystickInputDevice.h"
#include "JoystickSubsystem.h"
#include "Data/Input/KeyPair.h"
#include "Data/JoystickInformation.h"
#include "Data/JoystickType.h"
#include "Data/JoystickPowerLevel.h"
#include "Menus/AxisConfigurationEditor.h"
#include "Menus/ButtonConfigurationEditor.h"
#include "Widgets/AxisBar.h"
#include "Widgets/BallSwitch.h"
#include "Widgets/ButtonBox.h"
#include "Widgets/HatSwitch.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SListView.h"
#include "Styling/AppStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/SOverlay.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SWindow.h"
#include "Styling/CoreStyle.h"

void SJoystickInputViewer::Construct(const FArguments& InArgs, const TSharedRef<SDockTab>& ConstructUnderMajorTab, const TSharedPtr<SWindow>& ConstructUnderWindow)
{
	ParentWindow = ConstructUnderWindow;

	if (!GEngine)
	{
		return;
	}

	if (UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>())
	{
		JoystickSubsystem->Internal_JoystickPluggedInDelegate.AddRaw(this, &SJoystickInputViewer::JoystickPluggedIn);
		JoystickSubsystem->Internal_JoystickUnpluggedDelegate.AddRaw(this, &SJoystickInputViewer::JoystickUnplugged);
	}

	UpdateJoystickList();

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SNew(SVerticalBox)
			// Header Section with Device Selector
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(12, 12, 12, 8)
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
						.Text(FText::FromString("Device Selection"))
						.TextStyle(FAppStyle::Get(), "DetailsView.CategoryTextStyle")
						.Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						.Padding(0, 0, 8, 0)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Select Device:"))
							.TextStyle(FAppStyle::Get(), "NormalText")
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
						.MaxWidth(400.0f)
						[
							SAssignNew(DeviceComboBox, SComboBox<TSharedPtr<FJoystickInstanceId>>)
							.OptionsSource(&Joysticks)
							.OnGenerateWidget_Lambda([this](const TSharedPtr<FJoystickInstanceId>& InItem)
							{
								if (UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>())
								{
									FJoystickInformation JoystickInfo;
									if (JoystickSubsystem->GetJoystickInfo(*InItem, JoystickInfo))
									{
										return SNew(STextBlock)
											.Text(FText::FromString(*JoystickInfo.DeviceName))
											.TextStyle(FAppStyle::Get(), "NormalText");
									}
								}

								return SNew(STextBlock)
									.Text(FText::FromString("ERROR"))
									.TextStyle(FAppStyle::Get(), "NormalText");
							})
							.OnSelectionChanged_Lambda([this](const TSharedPtr<FJoystickInstanceId>& NewSelection, ESelectInfo::Type)
							{
								SelectedJoystick = NewSelection;
								UpdateCachedJoystickInfo();
								CreateWidgets();
							})
							.InitiallySelectedItem(SelectedJoystick)
							[
								SNew(STextBlock)
								.Text_Lambda([this]() -> FText
								{
									if (!SelectedJoystick.IsValid())
									{
										return FText::FromString("None");
									}

									return FText::FromString(*CachedJoystickInfo.DeviceName);
								})
								.TextStyle(FAppStyle::Get(), "NormalText")
							]
						]
					]
				]
			]


			// Content Area
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			.Padding(12, 0, 12, 12)
			[
				SNew(SScrollBox)
				+ SScrollBox::Slot()
				[
					SNew(SVerticalBox)
					// Device Information Section
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 0, 0, 8)
					[
						SNew(SBorder)
						.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
						.Padding(12)
						[
							SNew(SHorizontalBox)
							// Left Column: Device Information
							+ SHorizontalBox::Slot()
							.FillWidth(1.0f)
							.Padding(0, 0, 12, 0)
							[
								SNew(SVerticalBox)
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(0, 0, 0, 8)
								[
									SNew(STextBlock)
									.Text(FText::FromString("Device Information"))
									.TextStyle(FAppStyle::Get(), "DetailsView.CategoryTextStyle")
									.Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								[
									SNew(SVerticalBox)
									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(0, 2)
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										.Padding(0, 0, 8, 0)
										[
											SNew(STextBlock)
											.Text(FText::FromString("Type:"))
											.TextStyle(FAppStyle::Get(), "NormalText")
											.ColorAndOpacity(FSlateColor::UseSubduedForeground())
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(STextBlock)
											.Text_Lambda([this]() -> FText
											{
												if (!SelectedJoystick.IsValid())
												{
													return FText::FromString("N/A");
												}

												if (const UEnum* EnumPtr = StaticEnum<EJoystickType>())
												{
													FString EnumName = EnumPtr->GetNameStringByValue(static_cast<int64>(CachedJoystickInfo.Type));
													// Remove the "EJoystickType::" prefix
													EnumName.RemoveFromStart(TEXT("EJoystickType::"));
													return FText::FromString(EnumName);
												}
												return FText::FromString("Unknown");
											})
											.TextStyle(FAppStyle::Get(), "NormalText")
										]
									]
									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(0, 2)
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										.Padding(0, 0, 8, 0)
										[
											SNew(STextBlock)
											.Text(FText::FromString("Power Level:"))
											.TextStyle(FAppStyle::Get(), "NormalText")
											.ColorAndOpacity(FSlateColor::UseSubduedForeground())
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(STextBlock)
											.Text_Lambda([this]() -> FText
											{
												if (!SelectedJoystick.IsValid())
												{
													return FText::FromString("N/A");
												}

												if (const UEnum* EnumPtr = StaticEnum<EJoystickPowerLevel>())
												{
													FString EnumName = EnumPtr->GetNameStringByValue(static_cast<int64>(CachedJoystickInfo.PowerLevel));
													EnumName.RemoveFromStart(TEXT("EJoystickPowerLevel::"));
													return FText::FromString(EnumName);
												}
												return FText::FromString("Unknown");
											})
											.TextStyle(FAppStyle::Get(), "NormalText")
										]
									]
									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(0, 2)
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										.Padding(0, 0, 8, 0)
										[
											SNew(STextBlock)
											.Text(FText::FromString("Product ID:"))
											.TextStyle(FAppStyle::Get(), "NormalText")
											.ColorAndOpacity(FSlateColor::UseSubduedForeground())
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(STextBlock)
											.Text_Lambda([this]() -> FText
											{
												if (!SelectedJoystick.IsValid())
												{
													return FText::FromString("N/A");
												}

												return FText::FromString(FString::Printf(TEXT("0x%04X"), CachedJoystickInfo.ProductId));
											})
											.TextStyle(FAppStyle::Get(), "NormalText")
										]
									]
									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(0, 2)
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										.Padding(0, 0, 8, 0)
										[
											SNew(STextBlock)
											.Text(FText::FromString("Vendor ID:"))
											.TextStyle(FAppStyle::Get(), "NormalText")
											.ColorAndOpacity(FSlateColor::UseSubduedForeground())
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(STextBlock)
											.Text_Lambda([this]() -> FText
											{
												if (!SelectedJoystick.IsValid())
												{
													return FText::FromString("N/A");
												}

												return FText::FromString(FString::Printf(TEXT("0x%04X"), CachedJoystickInfo.VendorId));
											})
											.TextStyle(FAppStyle::Get(), "NormalText")
										]
									]
								]
							]
							// Vertical Separator
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.Padding(12, 0, 12, 0)
							[
								SNew(SSeparator)
								.Orientation(Orient_Vertical)
							]
							// Right Column: Supported Features
							+ SHorizontalBox::Slot()
							.FillWidth(1.0f)
							[
								SNew(SVerticalBox)
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(0, 0, 0, 8)
								[
									SNew(STextBlock)
									.Text(FText::FromString("Supported Features"))
									.TextStyle(FAppStyle::Get(), "DetailsView.CategoryTextStyle")
									.Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								[
									SNew(SVerticalBox)
									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(0, 2)
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										.Padding(0, 0, 8, 0)
										[
											SNew(STextBlock)
											.Text(FText::FromString("LED:"))
											.TextStyle(FAppStyle::Get(), "NormalText")
											.ColorAndOpacity(FSlateColor::UseSubduedForeground())
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(STextBlock)
											.Text_Lambda([this]() -> FText
											{
												if (!SelectedJoystick.IsValid())
												{
													return FText::FromString("N/A");
												}

												return FText::FromString(CachedJoystickInfo.LedSupport ? TEXT("Yes") : TEXT("No"));
											})
											.TextStyle(FAppStyle::Get(), "NormalText")
										]
									]
									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(0, 2)
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										.Padding(0, 0, 8, 0)
										[
											SNew(STextBlock)
											.Text(FText::FromString("Rumble:"))
											.TextStyle(FAppStyle::Get(), "NormalText")
											.ColorAndOpacity(FSlateColor::UseSubduedForeground())
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(STextBlock)
											.Text_Lambda([this]() -> FText
											{
												if (!SelectedJoystick.IsValid())
												{
													return FText::FromString("N/A");
												}

												return FText::FromString(CachedJoystickInfo.RumbleSupport ? TEXT("Yes") : TEXT("No"));
											})
											.TextStyle(FAppStyle::Get(), "NormalText")
										]
									]
									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(0, 2)
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										.Padding(0, 0, 8, 0)
										[
											SNew(STextBlock)
											.Text(FText::FromString("Haptic:"))
											.TextStyle(FAppStyle::Get(), "NormalText")
											.ColorAndOpacity(FSlateColor::UseSubduedForeground())
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(STextBlock)
											.Text_Lambda([this]() -> FText
											{
												if (!SelectedJoystick.IsValid())
												{
													return FText::FromString("N/A");
												}

												return FText::FromString(CachedJoystickInfo.Haptic.Supported ? TEXT("Yes") : TEXT("No"));
											})
											.TextStyle(FAppStyle::Get(), "NormalText")
										]
									]
									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(0, 2)
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										.Padding(0, 0, 8, 0)
										[
											SNew(STextBlock)
											.Text(FText::FromString("Gyro:"))
											.TextStyle(FAppStyle::Get(), "NormalText")
											.ColorAndOpacity(FSlateColor::UseSubduedForeground())
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(STextBlock)
											.Text_Lambda([this]() -> FText
											{
												if (!SelectedJoystick.IsValid())
												{
													return FText::FromString("N/A");
												}

												return FText::FromString(CachedJoystickInfo.Gyro.Supported ? TEXT("Yes") : TEXT("No"));
											})
											.TextStyle(FAppStyle::Get(), "NormalText")
										]
									]
									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(0, 2)
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										.Padding(0, 0, 8, 0)
										[
											SNew(STextBlock)
											.Text(FText::FromString("Accelerometer:"))
											.TextStyle(FAppStyle::Get(), "NormalText")
											.ColorAndOpacity(FSlateColor::UseSubduedForeground())
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(STextBlock)
											.Text_Lambda([this]() -> FText
											{
												if (!SelectedJoystick.IsValid())
												{
													return FText::FromString("N/A");
												}

												return FText::FromString(CachedJoystickInfo.Accelerometer.Supported ? TEXT("Yes") : TEXT("No"));
											})
											.TextStyle(FAppStyle::Get(), "NormalText")
										]
									]
									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(0, 2)
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										.Padding(0, 0, 8, 0)
										[
											SNew(STextBlock)
											.Text(FText::FromString("Game Controller:"))
											.TextStyle(FAppStyle::Get(), "NormalText")
											.ColorAndOpacity(FSlateColor::UseSubduedForeground())
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(STextBlock)
											.Text_Lambda([this]() -> FText
											{
												if (!SelectedJoystick.IsValid())
												{
													return FText::FromString("N/A");
												}

												return FText::FromString(CachedJoystickInfo.IsGameController ? TEXT("Yes") : TEXT("No"));
											})
											.TextStyle(FAppStyle::Get(), "NormalText")
										]
									]
								]
							]
						]
					]
					// Axes Section
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 0, 0, 8)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Axes"))
						.TextStyle(FAppStyle::Get(), "DetailsView.CategoryTextStyle")
						.Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 0, 0, 16)
					[
						SNew(SBorder)
						.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
						.Padding(12)
						[
							SAssignNew(AxisContainer, SHorizontalBox)
						]
					]

					// Buttons Section
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 0, 0, 8)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Buttons"))
						.TextStyle(FAppStyle::Get(), "DetailsView.CategoryTextStyle")
						.Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 0, 0, 16)
					[
						SNew(SBorder)
						.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
						.Padding(12)
						[
							SAssignNew(ButtonContainer, SWrapBox)
							.PreferredSize_Lambda([this]
							{
								return GetCachedGeometry().GetLocalSize().X - 48;
							})
							.InnerSlotPadding(FVector2D(8.0f, 8.0f))
							.Orientation(Orient_Horizontal)
							.HAlign(HAlign_Center)
						]
					]

					// Hat Switches Section
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 0, 0, 8)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Hat Switches"))
						.TextStyle(FAppStyle::Get(), "DetailsView.CategoryTextStyle")
						.Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 0, 0, 16)
					[
						SNew(SBorder)
						.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
						.Padding(12)
						[
							SAssignNew(HatContainer, SWrapBox)
							.PreferredSize_Lambda([this]
							{
								return GetCachedGeometry().GetLocalSize().X - 48;
							})
							.InnerSlotPadding(FVector2D(8.0f, 8.0f))
							.Orientation(Orient_Horizontal)
							.HAlign(HAlign_Left)
						]
					]

					// Hat Button Switches Section
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 0, 0, 8)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Hat Button Switches"))
						.TextStyle(FAppStyle::Get(), "DetailsView.CategoryTextStyle")
						.Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 0, 0, 16)
					[
						SNew(SBorder)
						.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
						.Padding(12)
						[
							SAssignNew(HatButtonContainer, SWrapBox)
							.PreferredSize_Lambda([this]
							{
								return GetCachedGeometry().GetLocalSize().X - 48;
							})
							.InnerSlotPadding(FVector2D(8.0f, 8.0f))
							.Orientation(Orient_Horizontal)
							.HAlign(HAlign_Left)
						]
					]

					// Ball Switches Section
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 0, 0, 8)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Ball Switches"))
						.TextStyle(FAppStyle::Get(), "DetailsView.CategoryTextStyle")
						.Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 0, 0, 16)
					[
						SNew(SBorder)
						.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
						.Padding(12)
						[
							SAssignNew(BallContainer, SWrapBox)
							.PreferredSize_Lambda([this]
							{
								return GetCachedGeometry().GetLocalSize().X - 48;
							})
							.InnerSlotPadding(FVector2D(8.0f, 8.0f))
							.Orientation(Orient_Horizontal)
							.HAlign(HAlign_Left)
						]
					]
				]
			]
		]
		+ SOverlay::Slot()
		[
			SAssignNew(InputBlocker, SBox)
			.Visibility(EVisibility::Collapsed)
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
				.BorderBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.3f))
			]
		]
	];

	CreateWidgets();
}

SJoystickInputViewer::~SJoystickInputViewer()
{
	if (UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>())
	{
		JoystickSubsystem->Internal_JoystickPluggedInDelegate.RemoveAll(this);
		JoystickSubsystem->Internal_JoystickUnpluggedDelegate.RemoveAll(this);
	}
}

void SJoystickInputViewer::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (!SelectedJoystick)
	{
		return;
	}

	if (!GEngine)
	{
		return;
	}

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	FJoystickDeviceState JoystickState;
	JoystickSubsystem->GetJoystickState(*SelectedJoystick, JoystickState);

	// Update visuals
	for (int i = 0; i < JoystickState.Axes.Num(); ++i)
	{
		if (JoystickState.Axes.IsValidIndex(i) && AxisBars.IsValidIndex(i))
		{
			AxisBars[i]->SetValue(JoystickState.Axes[i].GetValue());
		}
	}

	for (int i = 0; i < JoystickState.Buttons.Num(); ++i)
	{
		if (JoystickState.Buttons.IsValidIndex(i) && ButtonBoxes.IsValidIndex(i))
		{
			ButtonBoxes[i]->SetValue(JoystickState.Buttons[i].GetValue());
		}
	}

	for (int i = 0; i < JoystickState.Hats.Num(); ++i)
	{
		if (JoystickState.Hats.IsValidIndex(i) && HatSwitches.IsValidIndex(i))
		{
			HatSwitches[i]->SetValue(JoystickState.Hats[i].GetValue());
		}
	}

	for (int i = 0; i < JoystickState.Balls.Num(); ++i)
	{
		if (JoystickState.Balls.IsValidIndex(i) && BallSwitches.IsValidIndex(i))
		{
			BallSwitches[i]->SetValue(JoystickState.Balls[i].GetValue());
		}
	}
}

void SJoystickInputViewer::UpdateJoystickList()
{
	TArray<FJoystickInstanceId> InstanceIds;
	if (const UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>())
	{
		JoystickSubsystem->GetInstanceIds(InstanceIds);
	}

	if (InstanceIds.Num() == 0)
	{
		return;
	}

	Joysticks.Empty();
	for (const FJoystickInstanceId& Instance : InstanceIds)
	{
		AddJoystick(Instance, false);
	}

	SelectFirstJoystick();
	RefreshOptions();
}

void SJoystickInputViewer::CreateWidgets()
{
	if (!SelectedJoystick)
	{
		return;
	}

	if (!GEngine)
	{
		return;
	}

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	FJoystickDeviceState JoystickState;
	JoystickSubsystem->GetJoystickState(*SelectedJoystick, JoystickState);

	CreateAxisBars(JoystickSubsystem, JoystickState);
	CreateButtonBoxes(JoystickSubsystem, JoystickState);
	CreateHatSwitches(JoystickSubsystem, JoystickState);
	CreateBallSwitches(JoystickSubsystem, JoystickState);
}

void SJoystickInputViewer::CreateAxisBars(const UJoystickSubsystem* JoystickSubsystem, const FJoystickDeviceState& JoystickState)
{
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	if (!AxisContainer)
	{
		return;
	}

	AxisContainer->ClearChildren();
	AxisBars.Empty();

	for (int i = 0; i < JoystickState.Axes.Num(); i++)
	{
		const FAxisData& AxisValue = JoystickState.Axes[i];
		const FKey& Key = JoystickSubsystem->GetInputDevice()->GetDeviceAxisKey(*SelectedJoystick, i);

		TSharedPtr<SAxisBar> Bar;
		AxisContainer->AddSlot()
		             .Padding(5)
		[
			SAssignNew(Bar, SAxisBar)
			.Value(AxisValue.GetValue())
			.AxisIndex(i)
			.DisplayName(Key.GetDisplayName())
			.OnClicked(FSimpleDelegate::CreateLambda([this, Key, i]()
			{
				OpenAxisConfigurationEditor(Key, i);
			}))
		];
		AxisBars.Add(Bar);
	}
}

void SJoystickInputViewer::CreateButtonBoxes(const UJoystickSubsystem* JoystickSubsystem, const FJoystickDeviceState& JoystickState)
{
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	if (!ButtonContainer)
	{
		return;
	}

	ButtonContainer->ClearChildren();
	ButtonBoxes.Empty();

	for (int i = 0; i < JoystickState.Buttons.Num(); i++)
	{
		const FButtonData& ButtonValue = JoystickState.Buttons[i];
		const FKey& Key = JoystickSubsystem->GetInputDevice()->GetDeviceButtonKey(*SelectedJoystick, i);
		if (!Key.IsValid())
		{
			continue;
		}

		TSharedPtr<SButtonBox> Button;
		ButtonContainer->AddSlot()
		[
			SAssignNew(Button, SButtonBox)
			.Value(ButtonValue.GetValue())
			.ButtonIndex(i)
			.DisplayName(Key.GetDisplayName())
			.OnClicked(FSimpleDelegate::CreateLambda([this, Key, i]()
			{
				OpenButtonConfigurationEditor(Key, i);
			}))
		];
		ButtonBoxes.Add(Button);
	}
}

void SJoystickInputViewer::CreateHatSwitches(const UJoystickSubsystem* JoystickSubsystem, const FJoystickDeviceState& JoystickState)
{
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	if (!HatContainer)
	{
		return;
	}

	HatContainer->ClearChildren();
	HatSwitches.Empty();

	for (int i = 0; i < JoystickState.Hats.Num(); i++)
	{
		const FHatData& HatValue = JoystickState.Hats[i];
		const FKeyPair& Key = JoystickSubsystem->GetInputDevice()->GetDeviceHatKey(*SelectedJoystick, i);
		if (!Key.IsValid())
		{
			continue;
		}

		TSharedPtr<SHatSwitch> Hat;
		HatContainer->AddSlot()
		[
			SAssignNew(Hat, SHatSwitch)
			.Value(HatValue.GetValue())
			.HatIndex(i)
			.DisplayName(FText::FromString(FString::Format(TEXT("{0}\n{1}"), {Key.X.GetDisplayName().ToString(), Key.Y.GetDisplayName().ToString()})))
		];
		HatSwitches.Add(Hat);
	}
}

void SJoystickInputViewer::CreateBallSwitches(const UJoystickSubsystem* JoystickSubsystem, const FJoystickDeviceState& JoystickState)
{
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	if (!BallContainer)
	{
		return;
	}

	BallContainer->ClearChildren();
	BallSwitches.Empty();

	for (int i = 0; i < JoystickState.Balls.Num(); i++)
	{
		const FBallData& BallValue = JoystickState.Balls[i];

		TSharedPtr<SBallSwitch> Ball;
		BallContainer->AddSlot()
		[
			SAssignNew(Ball, SBallSwitch)
			.Value(BallValue.GetValue())
			.BallIndex(i)
		];
		BallSwitches.Add(Ball);
	}
}

void SJoystickInputViewer::JoystickPluggedIn(const FJoystickInstanceId& InstanceId)
{
	AddJoystick(InstanceId, true);
}

void SJoystickInputViewer::JoystickUnplugged(const FJoystickInstanceId& InstanceId)
{
	if (SelectedJoystick.IsValid() && *SelectedJoystick == InstanceId)
	{
		SelectedJoystick = nullptr;
		SelectFirstJoystick();
	}

	Joysticks.RemoveAll([InstanceId](const TSharedPtr<FJoystickInstanceId>& JoystickInstanceId)
	{
		if (!JoystickInstanceId.IsValid())
		{
			return false;
		}

		return *JoystickInstanceId == InstanceId;
	});

	RefreshOptions();
}

void SJoystickInputViewer::AddJoystick(const FJoystickInstanceId& InstanceId, const bool ForceRefreshOptions)
{
	Joysticks.Add(MakeShared<FJoystickInstanceId>(InstanceId));

	if (ForceRefreshOptions)
	{
		RefreshOptions();
	}
}

void SJoystickInputViewer::RefreshOptions() const
{
	if (DeviceComboBox)
	{
		DeviceComboBox->RefreshOptions();
	}
}

void SJoystickInputViewer::SelectFirstJoystick()
{
	if (SelectedJoystick)
	{
		return;
	}

	if (Joysticks.Num() == 0)
	{
		return;
	}

	SelectedJoystick = Joysticks[0];
	if (SelectedJoystick && DeviceComboBox)
	{
		DeviceComboBox->SetSelectedItem(SelectedJoystick);
	}
	UpdateCachedJoystickInfo();
}

void SJoystickInputViewer::OpenAxisConfigurationEditor(const FKey& AxisKey, const int AxisIndex) const
{
	if (!SelectedJoystick.IsValid())
	{
		return;
	}

	// Show input blocker
	if (InputBlocker.IsValid())
	{
		InputBlocker->SetVisibility(EVisibility::Visible);
	}

	const TSharedRef<SWindow> EditorWindow = SNew(SWindow)
		.Title(FText::Format(FText::FromString("Axis Configuration - {0}"), AxisKey.GetDisplayName()))
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		.FocusWhenFirstShown(true)
		.IsTopmostWindow(true)
		.SizingRule(ESizingRule::Autosized);

	const TSharedRef<SAxisConfigurationEditor> EditorWidget = SNew(SAxisConfigurationEditor)
		.AxisKey(AxisKey)
		.InstanceId(*SelectedJoystick)
		.AxisIndex(AxisIndex);

	EditorWindow->SetContent(EditorWidget);

	// Set callback to hide blocker when window closes
	EditorWindow->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>&)
	{
		if (InputBlocker.IsValid())
		{
			InputBlocker->SetVisibility(EVisibility::Collapsed);
		}
	}));

	FSlateApplication::Get().AddWindow(EditorWindow);
}

void SJoystickInputViewer::UpdateCachedJoystickInfo()
{
	if (!SelectedJoystick.IsValid())
	{
		CachedJoystickInfo = FJoystickInformation();
		return;
	}

	if (!GEngine)
	{
		CachedJoystickInfo = FJoystickInformation();
		return;
	}

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		CachedJoystickInfo = FJoystickInformation();
		return;
	}

	JoystickSubsystem->GetJoystickInfo(*SelectedJoystick, CachedJoystickInfo);
}

void SJoystickInputViewer::OpenButtonConfigurationEditor(const FKey& ButtonKey, const int ButtonIndex) const
{
	if (!SelectedJoystick.IsValid())
	{
		return;
	}

	// Show input blocker
	if (InputBlocker.IsValid())
	{
		InputBlocker->SetVisibility(EVisibility::Visible);
	}

	const TSharedRef<SWindow> EditorWindow = SNew(SWindow)
		.Title(FText::Format(FText::FromString("Button Configuration - {0}"), ButtonKey.GetDisplayName()))
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		.FocusWhenFirstShown(true)
		.IsTopmostWindow(true)
		.SizingRule(ESizingRule::Autosized);

	const TSharedRef<SButtonConfigurationEditor> EditorWidget = SNew(SButtonConfigurationEditor)
		.ButtonKey(ButtonKey)
		.InstanceId(*SelectedJoystick)
		.ButtonIndex(ButtonIndex);

	EditorWindow->SetContent(EditorWidget);

	// Set callback to hide blocker when window closes
	EditorWindow->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>&)
	{
		if (InputBlocker.IsValid())
		{
			InputBlocker->SetVisibility(EVisibility::Collapsed);
		}
	}));

	FSlateApplication::Get().AddWindow(EditorWindow);
}
