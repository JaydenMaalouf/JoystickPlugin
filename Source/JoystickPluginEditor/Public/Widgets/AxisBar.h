// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"

class SAxisBar final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAxisBar)
		{
		}

		SLATE_ARGUMENT(int, AxisIndex)
		SLATE_ARGUMENT(FText, DisplayName)
		SLATE_ARGUMENT(float, Value)
		SLATE_EVENT(FSimpleDelegate, OnClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetValue(const float InValue);

private:
	TAttribute<int> AxisIndex;
	TAttribute<FText> DisplayName;
	TAttribute<float> Value; // Ranges from -1 to 1
	FSimpleDelegate OnClickedDelegate;

	FText AxisText;

	FReply HandleClicked();
};
