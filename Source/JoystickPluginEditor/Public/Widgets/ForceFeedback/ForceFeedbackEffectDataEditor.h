// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/PropertyInformation.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Input/NumericTypeInterface.h"
#include "Widgets/Input/NumericUnitTypeInterface.inl"

class UForceFeedbackEffectBase;
class FProperty;

/**
 * Widget that displays sliders for all float properties in the EffectData
 */
class SForceFeedbackEffectDataEditor : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SForceFeedbackEffectDataEditor)
		{
		}

		SLATE_ARGUMENT(TWeakObjectPtr<UForceFeedbackEffectBase>, EffectPtr)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual ~SForceFeedbackEffectDataEditor() override;

	/** Exposes discovered float properties so the details customization can render one row per property. */
	const TArray<FPropertyInformation>& GetFloatProperties() const { return FloatProperties; }
	const TArray<FPropertyInformation>& GetIntProperties() const { return IntProperties; }
	const TArray<FPropertyInformation>& GetVectorProperties() const { return VectorProperties; }
	const TArray<FPropertyInformation>& GetVector2DProperties() const { return Vector2DProperties; }
	const TArray<FPropertyInformation>& GetIntVectorProperties() const { return IntVectorProperties; }

	template <typename TFProperty, typename T>
	TSharedRef<SWidget> CreateValueWidget(const FPropertyInformation& PropertyInfo);
	TSharedRef<SWidget> CreateVectorWidget(const FPropertyInformation& PropertyInfo);
	TSharedRef<SWidget> CreateVector2DWidget(const FPropertyInformation& PropertyInfo);
	TSharedRef<SWidget> CreateIntVectorWidget(const FPropertyInformation& PropertyInfo);

	/** Public method to create simulation effects (called by play button) */
	void CreateSimulationEffects();

	/** True when simulation effects are currently running/allocated */
	bool IsSimulating() const { return bIsSimulating; }

private:
	/** The effect being edited */
	TWeakObjectPtr<UForceFeedbackEffectBase> EffectPtr;

	/** True when preview simulation is toggled on */
	bool bIsSimulating = false;

	/** True after user modifies any virtual value via sliders */
	bool bVirtualDirty = false;

	/** The EffectData property found on the effect */
	FProperty* EffectDataProperty = nullptr;

	/** Pointer to the original EffectData struct in the effect object */
	void* OriginalEffectDataPtr = nullptr;

	/** Virtual copy of the EffectData struct for simulation */
	TArray<uint8> VirtualEffectData;

	/** Pointer to the virtual EffectData struct */
	void* VirtualEffectDataPtr = nullptr;

	/** The struct type of EffectData */
	const UScriptStruct* EffectDataStruct = nullptr;

	/** List of all float properties found in EffectData */
	TArray<FPropertyInformation> FloatProperties;
	TArray<FPropertyInformation> IntProperties;
	TArray<FPropertyInformation> VectorProperties;
	TArray<FPropertyInformation> Vector2DProperties;
	TArray<FPropertyInformation> IntVectorProperties;

	/** Copies the current EffectPtr's EffectData into the virtual struct */
	void SyncVirtualDataFromEffect();
	bool IsVectorType(const UScriptStruct* Struct) const;

	void FindProperties(const UStruct* Struct, void* StructData, const FString& Prefix, int32& DiscoveryOrder);

	template <class T>
	void GetPropertyMinMax(const FProperty* Property, float& OutMin, float& OutMax) const;

	/** Callback when slider value changes */
	template <typename TFProperty, typename T>
	void OnSliderValueChanged(T NewValue, const FPropertyInformation* PropertyInfo);

	/** Applies virtual data to all simulation effects */
	void ApplyVirtualDataToEffects();

	/** Destroys all simulation effects */
	void DestroySimulationEffects();
	void OnIntVectorValueChanged(FIntVector NewValue, const FPropertyInformation* PropertyInfo);
	void OnVectorValueChanged(FVector NewValue, const FPropertyInformation* PropertyInfo);
	void OnVector2DValueChanged(FVector2D NewValue, const FPropertyInformation* PropertyInfo);

	/** Array of effect instances created for simulation (one per FFB device) */
	TArray<TWeakObjectPtr<UForceFeedbackEffectBase>> SimulationEffects;
};

template <typename TFProperty, typename T>
TSharedRef<SWidget> SForceFeedbackEffectDataEditor::CreateValueWidget(const FPropertyInformation& PropertyInfo)
{
	TSharedRef<INumericTypeInterface<T>> PropertyUnits = MakeShareable<INumericTypeInterface<T>>(new TDefaultNumericTypeInterface<T>);
	if (PropertyInfo.Unit.IsSet())
	{
		PropertyUnits = MakeShared<TNumericUnitTypeInterface<T>>(PropertyInfo.Unit.GetValue());
	}

	const TWeakPtr<SForceFeedbackEffectDataEditor> WeakSelf = StaticCastSharedRef<SForceFeedbackEffectDataEditor>(AsShared());

	return SNew(SNumericEntryBox<T>)
		.Value_Lambda([WeakSelf, PropertyInfo]()
		{
			const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
			if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
			{
				return T{};
			}

			if (const TFProperty* Property = CastField<TFProperty>(PropertyInfo.Property))
			{
				return Property->GetPropertyValue(PropertyInfo.PropertyValuePtr);
			}

			return T{};
		})
		.OnValueChanged_Lambda([WeakSelf, PropertyInfo](const T NewValue)
		{
			const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
			if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
			{
				return;
			}

			Self->OnSliderValueChanged<TFProperty, T>(NewValue, &PropertyInfo);
		})
		.MinValue(PropertyInfo.MinValue)
		.MaxValue(PropertyInfo.MaxValue)
		.MinSliderValue(PropertyInfo.MinValue)
		.MaxSliderValue(PropertyInfo.MaxValue)
		.AllowSpin(true)
		.AllowWheel(true)
		.TypeInterface(PropertyUnits)
		.MinDesiredValueWidth(100.0f);
}

template <typename T>
void SForceFeedbackEffectDataEditor::GetPropertyMinMax(const FProperty* Property, float& OutMin, float& OutMax) const
{
	const T Min = TNumericLimits<T>::Lowest();
	const T Max = TNumericLimits<T>::Max();

	OutMin = static_cast<float>(Min);
	OutMax = static_cast<float>(Max);

	if (!Property)
	{
		return;
	}

	if (Property->HasMetaData(TEXT("UIMin")))
	{
		OutMin = Property->GetFloatMetaData(TEXT("UIMin"));
	}
	if (Property->HasMetaData(TEXT("UIMax")))
	{
		OutMax = Property->GetFloatMetaData(TEXT("UIMax"));
	}
}

template <typename TFProperty, typename T>
void SForceFeedbackEffectDataEditor::OnSliderValueChanged(T NewValue, const FPropertyInformation* PropertyInfo)
{
	if (!EffectPtr.IsValid() || !PropertyInfo || !PropertyInfo->PropertyValuePtr)
	{
		return;
	}

	if (const TFProperty* Property = CastField<TFProperty>(PropertyInfo->Property))
	{
		// Clamp the value to min/max
		NewValue = FMath::Clamp(NewValue, PropertyInfo->MinValue, PropertyInfo->MaxValue);

		// Set the value in the virtual copy (not the actual effect data)
		Property->SetPropertyValue(PropertyInfo->PropertyValuePtr, NewValue);
		bVirtualDirty = true;

		// Apply virtual data to all simulation effects
		ApplyVirtualDataToEffects();
	}
}
