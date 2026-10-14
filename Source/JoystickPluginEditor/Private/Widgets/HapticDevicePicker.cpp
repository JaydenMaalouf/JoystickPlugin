// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticDevicePicker.h"

#include "JoystickSubsystem.h"
#include "Data/JoystickInformation.h"
#include "Engine/Engine.h"
#include "Styling/AppStyle.h"
#include "Styling/JoystickPluginEditorStyle.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "HapticEffectEditor"

void SHapticDevicePicker::Construct(const FArguments& InArgs)
{
	OnDeviceChanged = InArgs._OnDeviceChanged;

	if (GEngine)
	{
		if (UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>())
		{
			JoystickSubsystem->Internal_JoystickPluggedInDelegate.AddRaw(this, &SHapticDevicePicker::JoystickPluggedIn);
			JoystickSubsystem->Internal_JoystickUnpluggedDelegate.AddRaw(this, &SHapticDevicePicker::JoystickUnplugged);
		}
	}

	UpdateJoystickList();

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(0, 0, 8, 0)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("DeviceLabel", "DEVICE"))
			.TextStyle(FJoystickPluginEditorStyle::Get(), "CaptionText")
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.MaxWidth(220.0f)
		[
			SAssignNew(DeviceComboBox, SComboBox<TSharedPtr<FJoystickInstanceId>>)
			.OptionsSource(&Joysticks)
			.OnGenerateWidget_Lambda([](const TSharedPtr<FJoystickInstanceId>& InItem) -> TSharedRef<SWidget>
			{
				return SNew(STextBlock)
					.Text(GetDeviceName(*InItem))
					.TextStyle(FAppStyle::Get(), "NormalText");
			})
			.OnSelectionChanged_Lambda([this](const TSharedPtr<FJoystickInstanceId>& NewSelection, ESelectInfo::Type)
			{
				SelectedJoystick = NewSelection;
				OnDeviceChanged.ExecuteIfBound();
			})
			[
				SNew(STextBlock)
				.Text_Lambda([this]() -> FText
				{
					return SelectedJoystick.IsValid() ? GetDeviceName(*SelectedJoystick) : LOCTEXT("NoDevice", "No Device Selected");
				})
				.TextStyle(FAppStyle::Get(), "NormalText")
			]
		]
	];
}

SHapticDevicePicker::~SHapticDevicePicker()
{
	if (GEngine)
	{
		if (UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>())
		{
			JoystickSubsystem->Internal_JoystickPluggedInDelegate.RemoveAll(this);
			JoystickSubsystem->Internal_JoystickUnpluggedDelegate.RemoveAll(this);
		}
	}
}

void SHapticDevicePicker::UpdateJoystickList()
{
	TArray<FJoystickInstanceId> InstanceIds;
	if (GEngine)
	{
		if (const UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>())
		{
			JoystickSubsystem->GetInstanceIds(InstanceIds);
		}
	}

	Joysticks.Empty();
	for (const FJoystickInstanceId& Instance : InstanceIds)
	{
		AddJoystick(Instance, false);
	}

	SelectFirstJoystick();
	RefreshDeviceOptions();
}

void SHapticDevicePicker::AddJoystick(const FJoystickInstanceId& InstanceId, const bool bForceRefreshOptions)
{
	Joysticks.Add(MakeShared<FJoystickInstanceId>(InstanceId));

	if (bForceRefreshOptions)
	{
		RefreshDeviceOptions();
	}
}

void SHapticDevicePicker::RefreshDeviceOptions() const
{
	if (DeviceComboBox)
	{
		DeviceComboBox->RefreshOptions();
	}
}

void SHapticDevicePicker::SelectFirstJoystick()
{
	if (SelectedJoystick.IsValid() || Joysticks.Num() == 0)
	{
		return;
	}

	SelectedJoystick = Joysticks[0];
	if (DeviceComboBox)
	{
		DeviceComboBox->SetSelectedItem(SelectedJoystick);
	}
}

void SHapticDevicePicker::JoystickPluggedIn(const FJoystickInstanceId& InstanceId)
{
	const bool bHadSelection = SelectedJoystick.IsValid();
	AddJoystick(InstanceId, true);
	SelectFirstJoystick();

	if (!bHadSelection && SelectedJoystick.IsValid())
	{
		OnDeviceChanged.ExecuteIfBound();
	}
}

void SHapticDevicePicker::JoystickUnplugged(const FJoystickInstanceId& InstanceId)
{
	Joysticks.RemoveAll([InstanceId](const TSharedPtr<FJoystickInstanceId>& JoystickInstanceId)
	{
		return JoystickInstanceId.IsValid() && *JoystickInstanceId == InstanceId;
	});

	RefreshDeviceOptions();

	if (SelectedJoystick.IsValid() && *SelectedJoystick == InstanceId)
	{
		SelectedJoystick = nullptr;
		SelectFirstJoystick();
		OnDeviceChanged.ExecuteIfBound();
	}
}

FText SHapticDevicePicker::GetDeviceName(const FJoystickInstanceId& InstanceId)
{
	FJoystickInformation JoystickInfo;
	if (GEngine)
	{
		if (UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>())
		{
			JoystickSubsystem->GetJoystickInfo(InstanceId, JoystickInfo);
		}
	}

	return FText::FromString(JoystickInfo.DeviceName);
}

#undef LOCTEXT_NAMESPACE
