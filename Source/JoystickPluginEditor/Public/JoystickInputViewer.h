// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"

struct FJoystickInstanceId;
struct FJoystickDeviceState;
class UJoystickSubsystem;
class SWrapBox;
class SBallSwitch;
class SHatSwitch;
class SButtonBox;
class SAxisBar;

class JOYSTICKPLUGINEDITOR_API SJoystickInputViewer final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SJoystickInputViewer)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<SDockTab>& ConstructUnderMajorTab, const TSharedPtr<SWindow>& ConstructUnderWindow);
	virtual ~SJoystickInputViewer() override;

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	TArray<TSharedPtr<FJoystickInstanceId>> Joysticks;
	TSharedPtr<FJoystickInstanceId> SelectedJoystick;
	TSharedPtr<SComboBox<TSharedPtr<FJoystickInstanceId>>> DeviceComboBox;

	TArray<TSharedPtr<SAxisBar>> AxisBars;
	TSharedPtr<SHorizontalBox> AxisContainer;

	TArray<TSharedPtr<SButtonBox>> ButtonBoxes;
	TSharedPtr<SWrapBox> ButtonContainer;

	TArray<TSharedPtr<SHatSwitch>> HatSwitches;
	TSharedPtr<SWrapBox> HatContainer;

	TArray<TSharedPtr<SButtonBox>> HatButtonSwitches;
	TSharedPtr<SWrapBox> HatButtonContainer;

	TArray<TSharedPtr<SBallSwitch>> BallSwitches;
	TSharedPtr<SWrapBox> BallContainer;

	FOnClicked OnJoystickUpdated;

	void UpdateJoystickList();

	void CreateWidgets();
	void CreateAxisBars(const UJoystickSubsystem* JoystickSubsystem, const FJoystickDeviceState& JoystickState);
	void CreateButtonBoxes(const UJoystickSubsystem* JoystickSubsystem, const FJoystickDeviceState& JoystickState);
	void CreateHatSwitches(const UJoystickSubsystem* JoystickSubsystem, const FJoystickDeviceState& JoystickState);
	void CreateBallSwitches(const UJoystickSubsystem* JoystickSubsystem, const FJoystickDeviceState& JoystickState);

	void JoystickPluggedIn(const FJoystickInstanceId& InstanceId);
	void JoystickUnplugged(const FJoystickInstanceId& InstanceId);

	void AddJoystick(const FJoystickInstanceId& InstanceId, const bool ForceRefreshOptions);
	void RefreshOptions() const;
	void SelectFirstJoystick();
};
