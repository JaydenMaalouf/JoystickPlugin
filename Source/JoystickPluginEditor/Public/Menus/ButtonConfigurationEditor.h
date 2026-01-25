// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "BaseKeyConfigurationEditor.h"
#include "Data/JoystickInstanceId.h"
#include "Widgets/SCompoundWidget.h"
#include "Data/Settings/JoystickInputDeviceButtonProperties.h"

class JOYSTICKPLUGINEDITOR_API SButtonConfigurationEditor final : public SBaseKeyConfigurationEditor
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
	FJoystickInputDeviceButtonProperties CurrentProperties;
	TSharedPtr<class SButtonBox> InputVisualiser;

	TSharedPtr<SCheckBox> InvertOutputCheckBox;

	TSharedPtr<SCheckBox> OverrideDisplayNameCheckBox;
	TSharedPtr<SEditableTextBox> DisplayNameTextBox;
	TSharedPtr<SHorizontalBox> DisplayNameInputContainer;

	// Track original display name state for restart notification
	FString OriginalDisplayName;
	bool bOriginalOverrideDisplayName = false;

	virtual void LoadConfiguration() override;
	virtual void SaveConfiguration() const override;
};
