// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Data/JoystickInstanceId.h"
#include "Widgets/SCompoundWidget.h"
#include "Data/Settings/JoystickInputDeviceButtonProperties.h"

class JOYSTICKPLUGINEDITOR_API SButtonConfigurationEditor final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SButtonConfigurationEditor)
		{
		}

		SLATE_ARGUMENT(FKey, ButtonKey)
		SLATE_ARGUMENT(FJoystickInstanceId, InstanceId)
		SLATE_ARGUMENT(int32, ButtonIndex)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SButtonConfigurationEditor() override;

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	FKey ButtonKey;
	FJoystickInstanceId InstanceId;
	int32 ButtonIndex;

	FJoystickInputDeviceButtonProperties CurrentProperties;
	TSharedPtr<class SButtonBox> InputVisualiser;

	// Simple property editors
	TSharedPtr<SCheckBox> InvertOutputCheckBox;

	// Display name override
	TSharedPtr<SCheckBox> OverrideDisplayNameCheckBox;
	TSharedPtr<class SEditableTextBox> DisplayNameTextBox;
	TSharedPtr<class SHorizontalBox> DisplayNameInputContainer;

	void LoadCurrentConfiguration();
	void SaveConfiguration() const;

	FReply OnSaveClicked();
	FReply OnCancelClicked();
};
