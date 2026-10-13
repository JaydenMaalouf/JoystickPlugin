// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Data/JoystickInstanceId.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SComboBox.h"

// The Haptic Effect Editor's device dropdown. Keeps its own list of connected joysticks up to date
// as they are plugged in and unplugged, selecting the first one whenever nothing is selected.
// OnDeviceChanged fires whenever the selected joystick changes, whether the user picked another or
// the selected one was unplugged.
class SHapticDevicePicker : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticDevicePicker)
		{
		}

		SLATE_EVENT(FSimpleDelegate, OnDeviceChanged)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SHapticDevicePicker() override;

	// Null while no joystick is connected.
	TSharedPtr<FJoystickInstanceId> GetSelectedJoystick() const { return SelectedJoystick; }

private:
	void UpdateJoystickList();
	void AddJoystick(const FJoystickInstanceId& InstanceId, bool bForceRefreshOptions);
	void RefreshDeviceOptions() const;
	void SelectFirstJoystick();
	void JoystickPluggedIn(const FJoystickInstanceId& InstanceId);
	void JoystickUnplugged(const FJoystickInstanceId& InstanceId);

	static FText GetDeviceName(const FJoystickInstanceId& InstanceId);

	TArray<TSharedPtr<FJoystickInstanceId>> Joysticks;
	TSharedPtr<FJoystickInstanceId> SelectedJoystick;
	TSharedPtr<SComboBox<TSharedPtr<FJoystickInstanceId>>> DeviceComboBox;
	FSimpleDelegate OnDeviceChanged;
};
