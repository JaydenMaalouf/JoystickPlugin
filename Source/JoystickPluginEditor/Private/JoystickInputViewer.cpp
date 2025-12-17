// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickInputViewer.h"

#include "JoystickInputDevice.h"
#include "JoystickSubsystem.h"
#include "Data/Input/KeyPair.h"
#include "Widgets/AxisBar.h"
#include "Widgets/BallSwitch.h"
#include "Widgets/ButtonBox.h"
#include "Widgets/HatSwitch.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SListView.h"

void SJoystickInputViewer::Construct(const FArguments& InArgs, const TSharedRef<SDockTab>& ConstructUnderMajorTab, const TSharedPtr<SWindow>& ConstructUnderWindow)
{
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
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight().Padding(10)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().Padding(4)
			[
				SNew(STextBlock).Text(FText::FromString("Select Device:"))
			]
			+ SHorizontalBox::Slot().AutoWidth()
			[
				SAssignNew(DeviceComboBox, SComboBox<TSharedPtr<FJoystickInstanceId>>)
				.OptionsSource(&Joysticks)
				.OnGenerateWidget_Lambda([](const TSharedPtr<FJoystickInstanceId>& InItem)
				{
					if (UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>())
					{
						FJoystickInformation JoystickInfo;
						if (JoystickSubsystem->GetJoystickInfo(*InItem, JoystickInfo))
						{
							return SNew(STextBlock).Text(FText::FromString(*JoystickInfo.DeviceName));
						}
					}

					return SNew(STextBlock).Text(FText::FromString("ERROR"));
				})
				.OnSelectionChanged_Lambda([this](const TSharedPtr<FJoystickInstanceId>& NewSelection, ESelectInfo::Type)
				{
					SelectedJoystick = NewSelection;
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

						UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
						if (!JoystickSubsystem)
						{
							return FText::FromString("None");
						}

						FJoystickInformation JoystickInfo;
						JoystickSubsystem->GetJoystickInfo(*SelectedJoystick, JoystickInfo);
						return FText::FromString(*JoystickInfo.DeviceName);
					})
				]
			]
		]

		+ SVerticalBox::Slot().Padding(10)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 5
				.MinHeight(500)
#endif
				.Padding(10) // Row 1, Column 0 – Change this to position it wherever you want
				[
					SAssignNew(AxisContainer, SHorizontalBox)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(10)
				[
					SAssignNew(ButtonContainer, SWrapBox)
					.PreferredSize_Lambda([this]
					{
						return GetCachedGeometry().GetLocalSize().X - 10;
					})
					.InnerSlotPadding(FVector2D(5.0f, 5.0f))
					.Orientation(Orient_Horizontal)
					.HAlign(HAlign_Center)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(10)
				[
					SAssignNew(HatContainer, SWrapBox)
					.PreferredSize_Lambda([this]
					{
						return GetCachedGeometry().GetLocalSize().X - 10;
					})
					.InnerSlotPadding(FVector2D(5.0f, 5.0f))
					.Orientation(Orient_Horizontal)
					.HAlign(HAlign_Center)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(HatButtonContainer, SWrapBox)
					.PreferredSize_Lambda([this]
					{
						return GetCachedGeometry().GetLocalSize().X - 10;
					})
					.InnerSlotPadding(FVector2D(5.0f, 5.0f))
					.Orientation(Orient_Horizontal)
					.HAlign(HAlign_Center)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(BallContainer, SWrapBox)
					.PreferredSize_Lambda([this]
					{
						return GetCachedGeometry().GetLocalSize().X - 10;
					})
					.InnerSlotPadding(FVector2D(5.0f, 5.0f))
					.Orientation(Orient_Horizontal)
					.HAlign(HAlign_Center)
				]
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
	if (!JoystickSubsystem)
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

	for (int i = 0; i < JoystickState.Hats.Num(); ++i)
	{
		if (JoystickState.Balls.IsValidIndex(i) && BallSwitches.IsValidIndex(i))
		{
			BallSwitches[i]->SetValue(JoystickState.Balls[i].GetValue());
		}
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
	if (!JoystickSubsystem)
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
	if (!JoystickSubsystem)
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
		];
		AxisBars.Add(Bar);
	}
}

void SJoystickInputViewer::CreateButtonBoxes(const UJoystickSubsystem* JoystickSubsystem, const FJoystickDeviceState& JoystickState)
{
	if (!JoystickSubsystem)
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
		];
		ButtonBoxes.Add(Button);
	}
}

void SJoystickInputViewer::CreateHatSwitches(const UJoystickSubsystem* JoystickSubsystem, const FJoystickDeviceState& JoystickState)
{
	if (!JoystickSubsystem)
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
	if (!JoystickSubsystem)
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
