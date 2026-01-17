// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Framework/Commands/InputChord.h"
#include "Fonts/SlateFontInfo.h"
#include "Layout/Margin.h"
#include "Widgets/SWidget.h"
#include "Components/Widget.h"
#include "Styling/SlateTypes.h"
#include "Layout/Visibility.h"

#include "JoystickInputSelector.generated.h"

class SJoystickInputSelector;
struct FButtonStyle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeySelected, FInputChord, SelectedKey);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIsSelectingChanged);

/** A widget for selecting a single key or a single key with a modifier. */
UCLASS()
class JOYSTICKPLUGIN_API UJoystickInputSelector : public UWidget
{
	GENERATED_BODY()

public:
	UJoystickInputSelector(const FObjectInitializer& ObjectInitializer);

	/** The button style used by the widget. */
	UPROPERTY(EditAnywhere, BlueprintSetter = SetButtonStyle, Category="Appearance", meta = (DisplayName = "Style"))
	FButtonStyle WidgetStyle;

	/** The text style used by the widget. */
	UPROPERTY(EditAnywhere, BlueprintSetter = SetTextStyle, Category="Appearance")
	FTextBlockStyle TextStyle;

	/** The currently selected key input. */
	UPROPERTY(EditAnywhere, BlueprintSetter = SetSelectedKey, FieldNotify, Category="Key Selection")
	FInputChord SelectedKey;

	/** The margin around the widget content. */
	UPROPERTY(EditAnywhere, BlueprintSetter = SetMargin, Category="Appearance")
	FMargin Margin;

	/** The text displayed while waiting for key selection. */
	UPROPERTY(EditAnywhere, BlueprintSetter = SetKeySelectionText, Category="Appearance")
	FText KeySelectionText;

	/** The text displayed when no key is specified. */
	UPROPERTY(EditAnywhere, BlueprintSetter = SetNoKeySpecifiedText, Category="Appearance")
	FText NoKeySpecifiedText;

	/** Bitmask of allowed input types (EInputSelectorTypes). */
	UPROPERTY(EditAnywhere, BlueprintSetter= SetInputSelectionType, Category="Key Selection", meta=(Bitmask, BitmaskEnum="/Script/JoystickPlugin.EInputSelectorTypes"))
	int32 AllowedInputTypes;

	/** Bitmask of allowed key types (EKeySelectorTypes). */
	UPROPERTY(EditAnywhere, BlueprintSetter= SetKeySelectionType, Category="Key Selection", meta=(Bitmask, BitmaskEnum="/Script/JoystickPlugin.EKeySelectorTypes"))
	int32 AllowedKeyTypes;

	/** Whether to use axis properties for range calculations instead of MinRange and MaxRange. */
	UPROPERTY(EditAnywhere, BlueprintSetter= SetUseAxisProperties, Category="Key Selection")
	bool bUseAxisProperties;

	/** The minimum range value for axis input. Only used when bUseAxisProperties is false. */
	UPROPERTY(EditAnywhere, BlueprintSetter= SetMinRange, Category="Key Selection", meta=(EditCondition="!bUseAxisProperties"))
	float MinRange;

	/** The maximum range value for axis input. Only used when bUseAxisProperties is false. */
	UPROPERTY(EditAnywhere, BlueprintSetter= SetMaxRange, Category="Key Selection", meta=(EditCondition="!bUseAxisProperties"))
	float MaxRange;

	/** The minimum range offset for axis input. */
	UPROPERTY(EditAnywhere, BlueprintSetter= SetMinRangeOffset, Category="Key Selection")
	float MinRangeOffset;

	/** The maximum range offset for axis input. */
	UPROPERTY(EditAnywhere, BlueprintSetter= SetMaxRangeOffset, Category="Key Selection")
	float MaxRangeOffset;

	/** The timeout duration for axis selection in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Key Selection")
	float AxisSelectionTimeout;

	/** The dead zone threshold for axis input. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Key Selection")
	float DeadZone;

	/** Keys that can be pressed to cancel key selection. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Key Selection")
	TArray<FKey> EscapeKeys;

	/** Called when a key is selected. */
	UPROPERTY(BlueprintAssignable, Category="Widget Event")
	FOnKeySelected OnKeySelected;

	/** Called whenever the key selection mode starts or stops. */
	UPROPERTY(BlueprintAssignable, Category="Widget Event")
	FOnIsSelectingChanged OnIsSelectingChanged;

	/** Sets the currently selected key. */
	UFUNCTION(BlueprintCallable, BlueprintSetter, Category="Key Selection")
	void SetSelectedKey(const FInputChord& InSelectedKey);

	/** Sets the text displayed while waiting for key selection. */
	UFUNCTION(BlueprintSetter, Category="Appearance")
	void SetKeySelectionText(FText InKeySelectionText);

	/** Sets the text displayed when no key is specified. */
	UFUNCTION(BlueprintSetter, Category="Appearance")
	void SetNoKeySpecifiedText(FText InNoKeySpecifiedText);

	/** Sets whether to use axis properties for range calculations. */
	UFUNCTION(BlueprintSetter, Category="Key Selection")
	void SetUseAxisProperties(bool bInUseAxisProperties);

	/** Sets the allowed input types (bitmask of EInputSelectorTypes). */
	UFUNCTION(BlueprintSetter, Category="Input Selection")
	void SetInputSelectionType(UPARAM(meta=(Bitmask, BitmaskEnum="/Script/JoystickPlugin.EInputSelectorTypes"))
		int32 NewMask);

	/** Sets the allowed key types (bitmask of EKeySelectorTypes). */
	UFUNCTION(BlueprintSetter, Category="Key Selection")
	void SetKeySelectionType(UPARAM(meta=(Bitmask, BitmaskEnum="/Script/JoystickPlugin.EKeySelectorTypes"))
		int32 NewMask);

	/** Sets the minimum range value for axis input. */
	UFUNCTION(BlueprintSetter, Category="Key Selection")
	void SetMinRange(float InMinRange);

	/** Sets the maximum range value for axis input. */
	UFUNCTION(BlueprintSetter, Category="Key Selection")
	void SetMaxRange(float InMaxRange);

	/** Sets the minimum range offset for axis input. */
	UFUNCTION(BlueprintSetter, Category="Key Selection")
	void SetMinRangeOffset(float InMinRangeOffset);

	/** Sets the maximum range offset for axis input. */
	UFUNCTION(BlueprintSetter, Category="Key Selection")
	void SetMaxRangeOffset(float InMaxRangeOffset);

	/** Sets the timeout duration for axis selection in seconds. */
	UFUNCTION(BlueprintSetter, Category="Key Selection")
	void SetAxisSelectionTimeout(float InAxisSelectionTimeout);

	/** Sets the dead zone threshold for axis input. */
	UFUNCTION(BlueprintSetter, Category="Key Selection")
	void SetDeadZone(float InDeadZone);

	/** Returns true if the widget is currently selecting a key, otherwise returns false. */
	UFUNCTION(BlueprintCallable, Category="Key Selection")
	bool GetIsSelectingKey() const;

	/** Sets the visibility of the text block. */
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Appearance")
	void SetTextBlockVisibility(const ESlateVisibility InVisibility) const;

	/** Sets the button style for the widget. */
	UFUNCTION(BlueprintSetter, Category="Appearance")
	void SetButtonStyle(const FButtonStyle& ButtonStyle);

	/** Sets the text style for the widget. */
	UFUNCTION(BlueprintSetter, Category="Appearance")
	void SetTextStyle(const FTextBlockStyle& InTextStyle);

	/** Sets the margin around the widget content. */
	UFUNCTION(BlueprintSetter, Category="Appearance")
	void SetMargin(const FMargin& InMargin);

	/** Sets escape keys. */
	UFUNCTION(BlueprintCallable, Category="Key Selection")
	void SetEscapeKeys(const TArray<FKey>& InKeys);

	//~ Begin UWidget Interface
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

protected:
	//~ Begin UObject Interface
	virtual void Serialize(FArchive& Ar) override;
	//~ End UObject Interface

	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

private:
	virtual void HandleKeySelected(const FInputChord& InSelectedKey);
	void HandleIsSelectingChanged() const;

	/** The input key selector widget managed by this object. */
	TSharedPtr<SJoystickInputSelector> JoystickInputSelector;
};
