// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticStatusChip.h"

#include "Fonts/FontMeasure.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/AppStyle.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

void SHapticStatusChip::Construct(const FArguments& InArgs)
{
	PlayheadTimeMilliseconds = InArgs._PlayheadTimeMilliseconds;
	ElapsedTimeMilliseconds = InArgs._ElapsedTimeMilliseconds;
	Force = InArgs._Force;
	Iteration = InArgs._Iteration;
	Iterations = InArgs._Iterations;

	const FSlateFontInfo& Font = FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText").Font;
	const TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();

	// Each readout sits in a box sized for its widest expected value, so the chip doesn't resize as the numbers change during playback.
	auto MakeReadout = [&Font, &FontMeasure](const TCHAR* WidestValue, const TAttribute<FText>& Text)
	{
		return SNew(SBox)
			.MinDesiredWidth(FontMeasure->Measure(WidestValue, Font).X)
			[
				SNew(STextBlock)
				.Text(Text)
				.TextStyle(FAppStyle::Get(), "NormalText")
			];
	};

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 12, 0)
		[
			MakeReadout(TEXT("t = 000.00s"), TAttribute<FText>::CreateLambda([this]
			{
				return FText::FromString(FString::Printf(TEXT("t = %.2fs"), PlayheadTimeMilliseconds.Get() / 1000.0));
			}))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 12, 0)
		[
			MakeReadout(TEXT("elapsed = 0000.00s"), TAttribute<FText>::CreateLambda([this]
			{
				return FText::FromString(FString::Printf(TEXT("elapsed = %.2fs"), ElapsedTimeMilliseconds.Get() / 1000.0));
			}))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 12, 0)
		[
			MakeReadout(TEXT("F = -0.00"), TAttribute<FText>::CreateLambda([this]
			{
				return FText::FromString(FString::Printf(TEXT("F = %.2f"), Force.Get()));
			}))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			MakeReadout(TEXT("i = 000/000"), TAttribute<FText>::CreateLambda([this]
			{
				const TOptional<int64> IterationsValue = Iterations.Get();
				const FString IterationCount = IterationsValue.IsSet() ? FString::Printf(TEXT("%lld"), IterationsValue.GetValue()) : FString::Chr(0x221E);

				return FText::FromString(FString::Printf(TEXT("i = %lld/%s"), Iteration.Get(), *IterationCount));
			}))
		]
	];
}
