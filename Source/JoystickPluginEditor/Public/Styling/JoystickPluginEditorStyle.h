// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Styling/SlateStyle.h"
#include "Templates/UniquePtr.h"

// The editor module's Slate style set, registered for as long as the module is loaded. Every colour the module's widgets use is set here, from the palette below.
class FJoystickPluginEditorStyle final : public FSlateStyleSet
{
public:
	static void Register();
	static void Unregister();

	static const ISlateStyle& Get();

private:
	FJoystickPluginEditorStyle();

	void SetHapticEffectEditorStyles();
	void SetInputViewerStyles();
	void SetDetailsStyles();

	static TUniquePtr<FJoystickPluginEditorStyle> Instance;

	static constexpr FLinearColor Green = FLinearColor(0.2f, 0.9f, 0.3f, 1.0f);
	static constexpr FLinearColor BrightGreen = FLinearColor(0.2f, 1.0f, 0.2f, 1.0f);
	static constexpr FLinearColor DimGreen = FLinearColor(0.2f, 0.8f, 0.2f, 1.0f);
	static constexpr FLinearColor Blue = FLinearColor(0.25f, 0.7f, 1.0f, 1.0f);
	static constexpr FLinearColor Red = FLinearColor(0.9f, 0.3f, 0.3f, 1.0f);
	static constexpr FLinearColor BrightRed = FLinearColor(1.0f, 0.2f, 0.2f, 1.0f);
	static constexpr FLinearColor DimRed = FLinearColor(0.8f, 0.2f, 0.2f, 1.0f);

	static constexpr FLinearColor Grey10 = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);
	static constexpr FLinearColor Grey15 = FLinearColor(0.15f, 0.15f, 0.15f, 1.0f);
	static constexpr FLinearColor Grey25 = FLinearColor(0.25f, 0.25f, 0.25f, 1.0f);
	static constexpr FLinearColor Grey40 = FLinearColor(0.4f, 0.4f, 0.4f, 1.0f);
	static constexpr FLinearColor Grey45 = FLinearColor(0.45f, 0.45f, 0.45f, 1.0f);
	static constexpr FLinearColor Grey50 = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
	static constexpr FLinearColor Grey60 = FLinearColor(0.6f, 0.6f, 0.6f, 1.0f);
	static constexpr FLinearColor Grey70 = FLinearColor(0.7f, 0.7f, 0.7f, 1.0f);
};
