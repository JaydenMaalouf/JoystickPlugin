// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickInputSelector.h"
#include "SJoystickInputSelector.h"
#include "Engine/Font.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/FrameworkObjectVersion.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Internationalization/Internationalization.h"

#define LOCTEXT_NAMESPACE "UMG"

static FButtonStyle* DefaultInputKeySelectorButtonStyle = nullptr;
static FTextBlockStyle* DefaultInputKeySelectorTextStyle = nullptr;

UJoystickInputSelector::UJoystickInputSelector(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (DefaultInputKeySelectorButtonStyle == nullptr)
	{
		// HACK: THIS SHOULD NOT COME FROM CORESTYLE AND SHOULD INSTEAD BE DEFINED BY ENGINE TEXTURES/PROJECT SETTINGS
		DefaultInputKeySelectorButtonStyle = new FButtonStyle(FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button"));

		// Unlink UMG default colors from the editor settings colors.
		DefaultInputKeySelectorButtonStyle->UnlinkColors();
	}

	if (DefaultInputKeySelectorTextStyle == nullptr)
	{
		// HACK: THIS SHOULD NOT COME FROM CORESTYLE AND SHOULD INSTEAD BE DEFINED BY ENGINE TEXTURES/PROJECT SETTINGS
		DefaultInputKeySelectorTextStyle = new FTextBlockStyle(
			FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText"));

		// Unlink UMG default colors from the editor settings colors.
		DefaultInputKeySelectorTextStyle->UnlinkColors();
	}

	WidgetStyle = *DefaultInputKeySelectorButtonStyle;
	TextStyle = *DefaultInputKeySelectorTextStyle;

	KeySelectionText = NSLOCTEXT("InputKeySelector", "DefaultKeySelectionText", "...");
	NoKeySpecifiedText = NSLOCTEXT("InputKeySelector", "DefaultEmptyText", "Empty");
	SelectedKey = FInputChord(EKeys::Invalid);
	bAllowAxisKeys = true;
	bAllowButtonKeys = true;
	bAllowModifierKeys = true;
	bAllowGamepadKeys = true;
	MinRangeOffset = 0.0f;
	MaxRangeOffset = 0.0f;
	AxisSelectionTimeout = 2.5f;

	EscapeKeys.AddUnique(EKeys::Gamepad_Special_Right);
	// In most (if not all) cases this is going to be the menu button

	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(*GetDefaultFontName());
		TextStyle.Font = FSlateFontInfo(RobotoFontObj.Object, 24, FName("Bold"));
	}
}

void UJoystickInputSelector::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar.UsingCustomVersion(FFrameworkObjectVersion::GUID);
}

void UJoystickInputSelector::PostLoad()
{
	Super::PostLoad();

	if (GetLinkerCustomVersion(FFrameworkObjectVersion::GUID) < FFrameworkObjectVersion::InputKeySelectorTextStyle)
	{
		TextStyle.Font = Font_DEPRECATED;
		TextStyle.ColorAndOpacity = ColorAndOpacity_DEPRECATED;
	}
}

void UJoystickInputSelector::SetSelectedKey(const FInputChord& InSelectedKey)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetSelectedKey(InSelectedKey);
	}

	SelectedKey = InSelectedKey;
}

void UJoystickInputSelector::SetKeySelectionText(FText InKeySelectionText)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetKeySelectionText(InKeySelectionText);
	}

	KeySelectionText = MoveTemp(InKeySelectionText);
}

void UJoystickInputSelector::SetNoKeySpecifiedText(FText InNoKeySpecifiedText)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetNoKeySpecifiedText(InNoKeySpecifiedText);
	}

	NoKeySpecifiedText = MoveTemp(InNoKeySpecifiedText);
}

void UJoystickInputSelector::SetAllowAxisKeys(bool bInAllowAxisKeys)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetAllowAxisKeys(bInAllowAxisKeys);
	}

	bAllowAxisKeys = bInAllowAxisKeys;
}

void UJoystickInputSelector::SetAllowButtonKeys(bool bInAllowButtonKeys)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetAllowButtonKeys(bInAllowButtonKeys);
	}

	bAllowButtonKeys = bInAllowButtonKeys;
}

void UJoystickInputSelector::SetAllowModifierKeys(const bool bInAllowModifierKeys)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetAllowModifierKeys(bInAllowModifierKeys);
	}

	bAllowModifierKeys = bInAllowModifierKeys;
}

void UJoystickInputSelector::SetAllowGamepadKeys(const bool bInAllowGamepadKeys)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetAllowGamepadKeys(bInAllowGamepadKeys);
	}

	bAllowGamepadKeys = bInAllowGamepadKeys;
}

void UJoystickInputSelector::SetMinRangeOffset(const float InMinRangeOffset)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetMinRangeOffset(InMinRangeOffset);
	}

	MinRangeOffset = InMinRangeOffset;
}

void UJoystickInputSelector::SetMaxRangeOffset(const float InMaxRangeOffset)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetMaxRangeOffset(InMaxRangeOffset);
	}

	MaxRangeOffset = InMaxRangeOffset;
}

void UJoystickInputSelector::SetAxisSelectionTimeout(float InAxisSelectionTimeout)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetAxisSelectionTimeout(InAxisSelectionTimeout);
	}

	AxisSelectionTimeout = InAxisSelectionTimeout;
}

bool UJoystickInputSelector::GetIsSelectingKey() const
{
	return JoystickInputSelector.IsValid() ? JoystickInputSelector->GetIsSelectingKey() : false;
}

void UJoystickInputSelector::SetButtonStyle(const FButtonStyle* InButtonStyle)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetButtonStyle(InButtonStyle);
	}
	WidgetStyle = *InButtonStyle;
}

void UJoystickInputSelector::SetEscapeKeys(const TArray<FKey>& InKeys)
{
	if (JoystickInputSelector.IsValid())
	{
		JoystickInputSelector->SetEscapeKeys(InKeys);
	}
	EscapeKeys = InKeys;
}
#if WITH_EDITOR
const FText UJoystickInputSelector::GetPaletteCategory()
{
	return LOCTEXT("Advanced", "Advanced");
}
#endif

void UJoystickInputSelector::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	JoystickInputSelector->SetSelectedKey(SelectedKey);
	JoystickInputSelector->SetMargin(Margin);
	JoystickInputSelector->SetButtonStyle(&WidgetStyle);
	JoystickInputSelector->SetTextStyle(&TextStyle);
	JoystickInputSelector->SetKeySelectionText(KeySelectionText);
	JoystickInputSelector->SetNoKeySpecifiedText(NoKeySpecifiedText);
	JoystickInputSelector->SetAllowModifierKeys(bAllowModifierKeys);
	JoystickInputSelector->SetAllowGamepadKeys(bAllowGamepadKeys);
	JoystickInputSelector->SetAllowAxisKeys(bAllowAxisKeys);
	JoystickInputSelector->SetAllowButtonKeys(bAllowButtonKeys);
	JoystickInputSelector->SetMinRangeOffset(MinRangeOffset);
	JoystickInputSelector->SetMaxRangeOffset(MaxRangeOffset);
	JoystickInputSelector->SetAxisSelectionTimeout(AxisSelectionTimeout);
	JoystickInputSelector->SetEscapeKeys(EscapeKeys);
}

void UJoystickInputSelector::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	JoystickInputSelector.Reset();
}

TSharedRef<SWidget> UJoystickInputSelector::RebuildWidget()
{
	JoystickInputSelector = SNew(SJoystickInputSelector)
		.SelectedKey(SelectedKey)
		.Margin(Margin)
		.ButtonStyle(&WidgetStyle)
		.TextStyle(&TextStyle)
		.KeySelectionText(KeySelectionText)
		.NoKeySpecifiedText(NoKeySpecifiedText)
		.AllowModifierKeys(bAllowModifierKeys)
		.AllowGamepadKeys(bAllowGamepadKeys)
		.AllowAxisKeys(bAllowAxisKeys)
		.AllowButtonKeys(bAllowButtonKeys)
		.SetMinRangeOffset(MinRangeOffset)
		.SetMaxRangeOffset(MaxRangeOffset)
		.SetAxisSelectionTimeout(AxisSelectionTimeout)
		.EscapeKeys(EscapeKeys)
		.OnAxisSelected(BIND_UOBJECT_DELEGATE(SJoystickInputSelector::FOnKeySelected, HandleAxisSelected))
		.OnKeySelected(BIND_UOBJECT_DELEGATE(SJoystickInputSelector::FOnKeySelected, HandleKeySelected))
		.OnIsSelectingChanged(BIND_UOBJECT_DELEGATE(SJoystickInputSelector::FOnIsSelectingChanged, HandleIsSelectingChanged));
	return JoystickInputSelector.ToSharedRef();
}

void UJoystickInputSelector::HandleAxisSelected(const FInputChord& InSelectedKey)
{
	SelectedKey = InSelectedKey;
	OnAxisSelected.Broadcast(SelectedKey);
}

void UJoystickInputSelector::HandleKeySelected(const FInputChord& InSelectedKey)
{
	SelectedKey = InSelectedKey;
	OnKeySelected.Broadcast(SelectedKey);
}

void UJoystickInputSelector::HandleIsSelectingChanged()
{
	OnIsSelectingChanged.Broadcast();
}

void UJoystickInputSelector::SetTextBlockVisibility(const ESlateVisibility InVisibility)
{
	if (JoystickInputSelector.IsValid())
	{
		EVisibility SlateVisibility = ConvertSerializedVisibilityToRuntime(InVisibility);
		JoystickInputSelector->SetTextBlockVisibility(SlateVisibility);
	}
}

#undef LOCTEXT_NAMESPACE
