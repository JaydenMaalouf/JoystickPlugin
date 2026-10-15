// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Styling/JoystickPluginEditorStyle.h"
#include "Styling/JoystickPluginEditorWidgetStyles.h"
#include "Widgets/SCompoundWidget.h"

class SAxisBar final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAxisBar)
			: _Style(&FJoystickPluginEditorStyle::Get().GetWidgetStyle<FAxisBarStyle>("AxisBar"))
		{
		}

		SLATE_STYLE_ARGUMENT(FAxisBarStyle, Style)
		SLATE_ARGUMENT(int, AxisIndex)
		SLATE_ARGUMENT(FText, DisplayName)
		SLATE_ARGUMENT(float, Value)
		SLATE_EVENT(FSimpleDelegate, OnClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetValue(const float InValue);

private:
	const FAxisBarStyle* Style = nullptr;
	TAttribute<int> AxisIndex;
	TAttribute<FText> DisplayName;
	TAttribute<float> Value; // Ranges from -1 to 1
	FSimpleDelegate OnClickedDelegate;

	FText AxisText;

	FReply HandleClicked() const;
};
