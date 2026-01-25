// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/ForceFeedback/ForceFeedbackEffectDataEditor.h"

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/Text/STextBlock.h"
#include "UObject/UnrealType.h"
#include "HAL/Platform.h"
#include "Engine/Engine.h"
#include "JoystickSubsystem.h"
#include "Data/JoystickInformation.h"
#include "Data/PropertyInformation.h"

#define LOCTEXT_NAMESPACE "ForceFeedbackEffectDataEditor"

void SForceFeedbackEffectDataEditor::Construct(const FArguments& InArgs)
{
	EffectPtr = InArgs._EffectPtr;

	EffectDataProperty = nullptr;
	OriginalEffectDataPtr = nullptr;
	VirtualEffectDataPtr = nullptr;
	EffectDataStruct = nullptr;
	bIsSimulating = false;
	bVirtualDirty = false;

	if (!EffectPtr.IsValid())
	{
		ChildSlot
		[
			SNew(STextBlock)
			.Text(LOCTEXT("InvalidEffect", "Invalid effect"))
		];
		return;
	}

	// Find the EffectData property on the actual class (not base class)
	const UClass* EffectClass = EffectPtr->GetClass();

	// Search for EffectData property in the class hierarchy
	for (const UClass* Class = EffectClass; Class && Class != UObject::StaticClass(); Class = Class->GetSuperClass())
	{
		EffectDataProperty = Class->FindPropertyByName(TEXT("EffectData"));
		if (EffectDataProperty)
		{
			break;
		}
	}

	if (!EffectDataProperty)
	{
		ChildSlot
		[
			SNew(STextBlock)
			.Text(LOCTEXT("NoEffectData", "No EffectData property found"))
		];
		return;
	}

	// Get pointer to the original EffectData struct in the effect object
	OriginalEffectDataPtr = EffectDataProperty->ContainerPtrToValuePtr<void>(EffectPtr.Get());
	if (!OriginalEffectDataPtr)
	{
		ChildSlot
		[
			SNew(STextBlock)
			.Text(LOCTEXT("NoEffectDataPtr", "Could not access EffectData"))
		];
		return;
	}

	// Get the struct type
	const FStructProperty* StructProp = CastField<FStructProperty>(EffectDataProperty);
	if (!StructProp || !StructProp->Struct)
	{
		ChildSlot
		[
			SNew(STextBlock)
			.Text(LOCTEXT("InvalidStruct", "EffectData is not a struct"))
		];
		return;
	}

	EffectDataStruct = StructProp->Struct;

	const int32 StructSize = EffectDataStruct->GetStructureSize();
	// Create a virtual copy of the EffectData struct for manipulation
	VirtualEffectData.SetNumUninitialized(StructSize);
	VirtualEffectDataPtr = VirtualEffectData.GetData();

	// Initialize and copy the struct
	EffectDataStruct->InitializeStruct(VirtualEffectDataPtr);
	EffectDataStruct->CopyScriptStruct(VirtualEffectDataPtr, OriginalEffectDataPtr);

	// Find all float properties recursively in the virtual copy
	int32 DiscoveryOrder = 0;
	FindProperties(EffectDataStruct, VirtualEffectDataPtr, FString(), DiscoveryOrder);

	if (FloatProperties.Num() == 0 || IntProperties.Num() == 0)
	{
		ChildSlot
		[
			SNew(STextBlock)
			.Text(LOCTEXT("NoProperties", "No properties found in EffectData"))
		];
		return;
	}

	// UI is provided by the details customization (one details row per property).
	ChildSlot
	[
		SNullWidget::NullWidget
	];
}

SForceFeedbackEffectDataEditor::~SForceFeedbackEffectDataEditor()
{
	// Destroy all simulation effects
	DestroySimulationEffects();

	// Clean up the virtual struct
	if (VirtualEffectDataPtr && EffectDataStruct)
	{
		EffectDataStruct->DestroyStruct(VirtualEffectDataPtr);
	}
}

TSharedRef<SWidget> SForceFeedbackEffectDataEditor::CreateVectorWidget(const FPropertyInformation& PropertyInfo)
{
	const TWeakPtr<SForceFeedbackEffectDataEditor> WeakSelf = StaticCastSharedRef<SForceFeedbackEffectDataEditor>(AsShared());

	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f, 0.0f)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("X", "X:"))
				.MinDesiredWidth(15.0f)
			]
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.Padding(2.0f, 0.0f)
		[
			SNew(SNumericEntryBox<double>)
			.Value_Lambda([WeakSelf, PropertyInfo]()
			{
				const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
				if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
				{
					return 0.0;
				}
				const FVector* VectorPtr = static_cast<const FVector*>(PropertyInfo.PropertyValuePtr);
				return VectorPtr ? VectorPtr->X : 0.0;
			})
			.OnValueChanged_Lambda([WeakSelf, PropertyInfo](const float NewValue)
			{
				const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
				if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
				{
					return;
				}
				if (const FVector* VectorPtr = static_cast<FVector*>(PropertyInfo.PropertyValuePtr))
				{
					FVector NewVector = *VectorPtr;
					NewVector.X = FMath::Clamp(NewValue, PropertyInfo.MinValue, PropertyInfo.MaxValue);
					Self->OnVectorValueChanged(NewVector, &PropertyInfo);
				}
			})
			.MinValue(PropertyInfo.MinValue)
			.MaxValue(PropertyInfo.MaxValue)
			.MinSliderValue(PropertyInfo.MinValue)
			.MaxSliderValue(PropertyInfo.MaxValue)
			.AllowSpin(true)
			.MinDesiredValueWidth(80.0f)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f, 0.0f)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Y", "Y:"))
				.MinDesiredWidth(15.0f)
			]
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.Padding(2.0f, 0.0f)
		[
			SNew(SNumericEntryBox<double>)
			.Value_Lambda([WeakSelf, PropertyInfo]()
			{
				const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
				if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
				{
					return 0.0;
				}
				const FVector* VectorPtr = static_cast<const FVector*>(PropertyInfo.PropertyValuePtr);
				return VectorPtr ? VectorPtr->Y : 0.0;
			})
			.OnValueChanged_Lambda([WeakSelf, PropertyInfo](const float NewValue)
			{
				const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
				if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
				{
					return;
				}
				if (const FVector* VectorPtr = static_cast<FVector*>(PropertyInfo.PropertyValuePtr))
				{
					FVector NewVector = *VectorPtr;
					NewVector.Y = FMath::Clamp(NewValue, PropertyInfo.MinValue, PropertyInfo.MaxValue);
					Self->OnVectorValueChanged(NewVector, &PropertyInfo);
				}
			})
			.MinValue(PropertyInfo.MinValue)
			.MaxValue(PropertyInfo.MaxValue)
			.MinSliderValue(PropertyInfo.MinValue)
			.MaxSliderValue(PropertyInfo.MaxValue)
			.AllowSpin(true)
			.MinDesiredValueWidth(80.0f)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f, 0.0f)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Z", "Z:"))
				.MinDesiredWidth(15.0f)
			]
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.Padding(2.0f, 0.0f)
		[
			SNew(SNumericEntryBox<double>)
			.Value_Lambda([WeakSelf, PropertyInfo]()
			{
				const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
				if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
				{
					return 0.0;
				}
				const FVector* VectorPtr = static_cast<const FVector*>(PropertyInfo.PropertyValuePtr);
				return VectorPtr ? VectorPtr->Z : 0.0;
			})
			.OnValueChanged_Lambda([WeakSelf, PropertyInfo](const float NewValue)
			{
				const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
				if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
				{
					return;
				}
				if (const FVector* VectorPtr = static_cast<FVector*>(PropertyInfo.PropertyValuePtr))
				{
					FVector NewVector = *VectorPtr;
					NewVector.Z = FMath::Clamp(NewValue, PropertyInfo.MinValue, PropertyInfo.MaxValue);
					Self->OnVectorValueChanged(NewVector, &PropertyInfo);
				}
			})
			.MinValue(PropertyInfo.MinValue)
			.MaxValue(PropertyInfo.MaxValue)
			.MinSliderValue(PropertyInfo.MinValue)
			.MaxSliderValue(PropertyInfo.MaxValue)
			.AllowSpin(true)
			.MinDesiredValueWidth(80.0f)
		];
}

TSharedRef<SWidget> SForceFeedbackEffectDataEditor::CreateVector2DWidget(const FPropertyInformation& PropertyInfo)
{
	const TWeakPtr<SForceFeedbackEffectDataEditor> WeakSelf = StaticCastSharedRef<SForceFeedbackEffectDataEditor>(AsShared());

	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f, 0.0f)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("X", "X:"))
				.MinDesiredWidth(15.0f)
			]
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.Padding(2.0f, 0.0f)
		[
			SNew(SNumericEntryBox<double>)
			.Value_Lambda([WeakSelf, PropertyInfo]()
			{
				const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
				if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
				{
					return 0.0;
				}
				const FVector2D* VectorPtr = static_cast<const FVector2D*>(PropertyInfo.PropertyValuePtr);
				return VectorPtr ? VectorPtr->X : 0.0;
			})
			.OnValueChanged_Lambda([WeakSelf, PropertyInfo](const float NewValue)
			{
				const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
				if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
				{
					return;
				}
				if (const FVector2D* VectorPtr = static_cast<FVector2D*>(PropertyInfo.PropertyValuePtr))
				{
					FVector2D NewVector = *VectorPtr;
					NewVector.X = FMath::Clamp(NewValue, PropertyInfo.MinValue, PropertyInfo.MaxValue);
					Self->OnVector2DValueChanged(NewVector, &PropertyInfo);
				}
			})
			.MinValue(PropertyInfo.MinValue)
			.MaxValue(PropertyInfo.MaxValue)
			.MinSliderValue(PropertyInfo.MinValue)
			.MaxSliderValue(PropertyInfo.MaxValue)
			.AllowSpin(true)
			.MinDesiredValueWidth(80.0f)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f, 0.0f)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Y", "Y:"))
				.MinDesiredWidth(15.0f)
			]
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.Padding(2.0f, 0.0f)
		[
			SNew(SNumericEntryBox<double>)
			.Value_Lambda([WeakSelf, PropertyInfo]()
			{
				const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
				if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
				{
					return 0.0;
				}
				const FVector2D* VectorPtr = static_cast<const FVector2D*>(PropertyInfo.PropertyValuePtr);
				return VectorPtr ? VectorPtr->Y : 0.0;
			})
			.OnValueChanged_Lambda([WeakSelf, PropertyInfo](const float NewValue)
			{
				const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
				if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
				{
					return;
				}
				if (const FVector2D* VectorPtr = static_cast<FVector2D*>(PropertyInfo.PropertyValuePtr))
				{
					FVector2D NewVector = *VectorPtr;
					NewVector.Y = FMath::Clamp(NewValue, PropertyInfo.MinValue, PropertyInfo.MaxValue);
					Self->OnVector2DValueChanged(NewVector, &PropertyInfo);
				}
			})
			.MinValue(PropertyInfo.MinValue)
			.MaxValue(PropertyInfo.MaxValue)
			.MinSliderValue(PropertyInfo.MinValue)
			.MaxSliderValue(PropertyInfo.MaxValue)
			.AllowSpin(true)
			.MinDesiredValueWidth(80.0f)
		];
}

TSharedRef<SWidget> SForceFeedbackEffectDataEditor::CreateIntVectorWidget(const FPropertyInformation& PropertyInfo)
{
	const TWeakPtr<SForceFeedbackEffectDataEditor> WeakSelf = StaticCastSharedRef<SForceFeedbackEffectDataEditor>(AsShared());

	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f, 0.0f)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("X", "X:"))
				.MinDesiredWidth(15.0f)
			]
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.Padding(2.0f, 0.0f)
		[
			SNew(SNumericEntryBox<int>)
			.Value_Lambda([WeakSelf, PropertyInfo]()
			{
				const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
				if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
				{
					return 0;
				}
				const FIntVector* VectorPtr = static_cast<const FIntVector*>(PropertyInfo.PropertyValuePtr);
				return VectorPtr ? VectorPtr->X : 0;
			})
			.OnValueChanged_Lambda([WeakSelf, PropertyInfo](const int NewValue)
			{
				const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
				if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
				{
					return;
				}
				if (const FIntVector* VectorPtr = static_cast<FIntVector*>(PropertyInfo.PropertyValuePtr))
				{
					FIntVector NewVector = *VectorPtr;
					NewVector.X = FMath::Clamp(NewValue, PropertyInfo.MinValue, PropertyInfo.MaxValue);
					Self->OnIntVectorValueChanged(NewVector, &PropertyInfo);
				}
			})
			.MinValue(PropertyInfo.MinValue)
			.MaxValue(PropertyInfo.MaxValue)
			.MinSliderValue(PropertyInfo.MinValue)
			.MaxSliderValue(PropertyInfo.MaxValue)
			.AllowSpin(true)
			.MinDesiredValueWidth(80.0f)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f, 0.0f)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Y", "Y:"))
				.MinDesiredWidth(15.0f)
			]
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.Padding(2.0f, 0.0f)
		[
			SNew(SNumericEntryBox<int>)
			.Value_Lambda([WeakSelf, PropertyInfo]()
			{
				const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
				if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
				{
					return 0;
				}
				const FIntVector* VectorPtr = static_cast<const FIntVector*>(PropertyInfo.PropertyValuePtr);
				return VectorPtr ? VectorPtr->Y : 0;
			})
			.OnValueChanged_Lambda([WeakSelf, PropertyInfo](const int NewValue)
			{
				const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
				if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
				{
					return;
				}
				if (const FIntVector* VectorPtr = static_cast<FIntVector*>(PropertyInfo.PropertyValuePtr))
				{
					FIntVector NewVector = *VectorPtr;
					NewVector.Y = FMath::Clamp(NewValue, PropertyInfo.MinValue, PropertyInfo.MaxValue);
					Self->OnIntVectorValueChanged(NewVector, &PropertyInfo);
				}
			})
			.MinValue(PropertyInfo.MinValue)
			.MaxValue(PropertyInfo.MaxValue)
			.MinSliderValue(PropertyInfo.MinValue)
			.MaxSliderValue(PropertyInfo.MaxValue)
			.AllowSpin(true)
			.MinDesiredValueWidth(80.0f)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f, 0.0f)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Z", "Z:"))
				.MinDesiredWidth(15.0f)
			]
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.Padding(2.0f, 0.0f)
		[
			SNew(SNumericEntryBox<int>)
			.Value_Lambda([WeakSelf, PropertyInfo]()
			{
				const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
				if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
				{
					return 0;
				}
				const FIntVector* VectorPtr = static_cast<const FIntVector*>(PropertyInfo.PropertyValuePtr);
				return VectorPtr ? VectorPtr->Z : 0;
			})
			.OnValueChanged_Lambda([WeakSelf, PropertyInfo](const int NewValue)
			{
				const TSharedPtr<SForceFeedbackEffectDataEditor> Self = WeakSelf.Pin();
				if (!Self.IsValid() || !Self->EffectPtr.IsValid() || !PropertyInfo.PropertyValuePtr)
				{
					return;
				}

				if (FIntVector* VectorPtr = static_cast<FIntVector*>(PropertyInfo.PropertyValuePtr))
				{
					VectorPtr->Z = FMath::Clamp(NewValue, PropertyInfo.MinValue, PropertyInfo.MaxValue);
				}
			})
			.MinValue(TNumericLimits<int>::Lowest())
			.MaxValue(TNumericLimits<int>::Max())
			.MinSliderValue(TNumericLimits<int>::Lowest())
			.MaxSliderValue(TNumericLimits<int>::Max())
			.AllowSpin(true)
		];
}

void SForceFeedbackEffectDataEditor::CreateSimulationEffects()
{
	// Toggle simulation on/off. State is tracked explicitly, not inferred from effect count.
	if (bIsSimulating)
	{
		bIsSimulating = false;
		DestroySimulationEffects(); // force stop + destroy + empty array
		return;
	}

	if (!EffectPtr.IsValid() || !GEngine)
	{
		bIsSimulating = false;
		return;
	}

	// If the user hasn't tweaked sliders, refresh the virtual data from the effect so
	// base/details edits made after widget construction aren't stale during preview.
	if (!bVirtualDirty)
	{
		SyncVirtualDataFromEffect();
	}

	// Get the joystick subsystem
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		bIsSimulating = false;
		return;
	}

	// Get all connected device instance IDs
	TArray<FJoystickInstanceId> InstanceIds;
	JoystickSubsystem->GetInstanceIds(InstanceIds, false); // false = only connected devices

	// Get the effect class type
	const UClass* EffectClass = EffectPtr->GetClass();

	// Create an effect instance for each FFB-capable device
	for (const FJoystickInstanceId& InstanceId : InstanceIds)
	{
		// Check if device has haptic support
		FJoystickInformation JoystickInfo;
		if (!JoystickSubsystem->GetJoystickInfo(InstanceId, JoystickInfo))
		{
			continue;
		}

		if (!JoystickInfo.Haptic.Supported)
		{
			continue;
		}

		// Create a new effect instance of the same type
		UForceFeedbackEffectBase* NewEffect = NewObject<UForceFeedbackEffectBase>(
			EffectPtr->GetOuter(),
			EffectClass,
			NAME_None,
			RF_Transient // Transient so it's not saved
		);

		if (!IsValid(NewEffect))
		{
			continue;
		}

		// Copy configuration from original effect
		NewEffect->Configuration = EffectPtr->Configuration;
		NewEffect->Iterations = EffectPtr->Iterations;
		NewEffect->InfiniteIterations = EffectPtr->InfiniteIterations;
		NewEffect->Tickable = EffectPtr->Tickable;
		NewEffect->TickableInEditor = EffectPtr->TickableInEditor;
		NewEffect->TickableWhenPaused = EffectPtr->TickableWhenPaused;
		NewEffect->ForceStopAfterDurationLapsed = EffectPtr->ForceStopAfterDurationLapsed;

		// Copy the EffectData from the original
		const FProperty* OriginalEffectDataProp = EffectDataProperty;
		const FProperty* NewEffectDataProp = nullptr;

		const UClass* NewEffectClass = NewEffect->GetClass();
		for (const UClass* Class = NewEffectClass; Class && Class != UObject::StaticClass(); Class = Class->GetSuperClass())
		{
			NewEffectDataProp = Class->FindPropertyByName(TEXT("EffectData"));
			if (NewEffectDataProp)
			{
				break;
			}
		}

		if (NewEffectDataProp && OriginalEffectDataProp)
		{
			void* NewEffectDataPtr = NewEffectDataProp->ContainerPtrToValuePtr<void>(NewEffect);
			if (NewEffectDataPtr && OriginalEffectDataPtr)
			{
				// Use the current virtual data so CreateEffect() (called by InitialiseEffect)
				// is built from the latest edited values at the moment Play is pressed.
				if (VirtualEffectDataPtr)
				{
					EffectDataStruct->CopyScriptStruct(NewEffectDataPtr, VirtualEffectDataPtr);
				}
				else
				{
					EffectDataStruct->CopyScriptStruct(NewEffectDataPtr, OriginalEffectDataPtr);
				}
			}
		}

		// Set the instance ID and initialize
		NewEffect->SetInstanceId(InstanceId);
		NewEffect->InitialiseEffect();
		NewEffect->StartEffect();

		// Add to simulation effects array
		SimulationEffects.Add(NewEffect);
	}

	// Ensure the simulation reflects the current edited (virtual) values, not just the original data.
	ApplyVirtualDataToEffects();

	// Turning simulation on.
	bIsSimulating = true;
}

void SForceFeedbackEffectDataEditor::SyncVirtualDataFromEffect()
{
	if (!EffectPtr.IsValid() || !OriginalEffectDataPtr || !VirtualEffectDataPtr || !EffectDataStruct)
	{
		return;
	}

	EffectDataStruct->CopyScriptStruct(VirtualEffectDataPtr, OriginalEffectDataPtr);
	bVirtualDirty = false;
}

bool SForceFeedbackEffectDataEditor::IsVectorType(const UScriptStruct* Struct) const
{
	if (!Struct)
	{
		return false;
	}

	// Check against known vector types
	return Struct == TBaseStructure<FIntVector>::Get() ||
		Struct == TBaseStructure<FVector>::Get() ||
		Struct == TBaseStructure<FVector2D>::Get();
}

void SForceFeedbackEffectDataEditor::FindProperties(const UStruct* Struct, void* StructData, const FString& Prefix, int32& DiscoveryOrder)
{
	if (!Struct || !StructData)
	{
		return;
	}

	// Iterate through all properties in the struct
	for (TFieldIterator<FProperty> PropIt(Struct); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;
		if (!Property)
		{
			continue;
		}

		// Get pointer to this property's data
		void* PropertyData = Property->ContainerPtrToValuePtr<void>(StructData);
		if (const FStructProperty* StructProp = CastField<FStructProperty>(Property))
		{
			if (StructProp->Struct && PropertyData)
			{
				if (IsVectorType(StructProp->Struct))
				{
					// Handle FIntVector
					if (StructProp->Struct == TBaseStructure<FIntVector>::Get())
					{
						FPropertyInformation PropInfo;
						PropInfo.Property = Property;
						PropInfo.PropertyValuePtr = PropertyData;
						PropInfo.DisplayName = Property->GetDisplayNameText().ToString();
						PropInfo.PropertyPath = Prefix.IsEmpty() ? Property->GetName() : FString::Printf(TEXT("%s.%s"), *Prefix, *Property->GetName());
						const auto PropertyUnits = Property->GetMetaData("Units");
						PropInfo.Unit = FUnitConversion::UnitFromString(*PropertyUnits);
						PropInfo.DiscoveryOrder = DiscoveryOrder++;
						GetPropertyMinMax<int>(Property, PropInfo.MinValue, PropInfo.MaxValue);
						IntVectorProperties.Add(PropInfo);
					}
					// Handle FVector
					else if (StructProp->Struct == TBaseStructure<FVector>::Get())
					{
						FPropertyInformation PropInfo;
						PropInfo.Property = Property;
						PropInfo.PropertyValuePtr = PropertyData;
						PropInfo.DisplayName = Property->GetDisplayNameText().ToString();
						PropInfo.PropertyPath = Prefix.IsEmpty() ? Property->GetName() : FString::Printf(TEXT("%s.%s"), *Prefix, *Property->GetName());
						const auto PropertyUnits = Property->GetMetaData("Units");
						PropInfo.Unit = FUnitConversion::UnitFromString(*PropertyUnits);
						PropInfo.DiscoveryOrder = DiscoveryOrder++;
						GetPropertyMinMax<float>(Property, PropInfo.MinValue, PropInfo.MaxValue);
						VectorProperties.Add(PropInfo);
					}
					// Handle FVector2D
					else if (StructProp->Struct == TBaseStructure<FVector2D>::Get())
					{
						FPropertyInformation PropInfo;
						PropInfo.Property = Property;
						PropInfo.PropertyValuePtr = PropertyData;
						PropInfo.DisplayName = Property->GetDisplayNameText().ToString();
						PropInfo.PropertyPath = Prefix.IsEmpty() ? Property->GetName() : FString::Printf(TEXT("%s.%s"), *Prefix, *Property->GetName());
						const auto PropertyUnits = Property->GetMetaData("Units");
						PropInfo.Unit = FUnitConversion::UnitFromString(*PropertyUnits);
						PropInfo.DiscoveryOrder = DiscoveryOrder++;
						GetPropertyMinMax<float>(Property, PropInfo.MinValue, PropInfo.MaxValue);
						Vector2DProperties.Add(PropInfo);
					}
				}
				else
				{
					FString NewPrefix = Prefix.IsEmpty() ? Property->GetName() : FString::Printf(TEXT("%s.%s"), *Prefix, *Property->GetName());
					FindProperties(StructProp->Struct, PropertyData, NewPrefix, DiscoveryOrder);
				}
			}
		}
		else if (const FFloatProperty* FloatProp = CastField<FFloatProperty>(Property))
		{
			FPropertyInformation PropInfo;
			PropInfo.Property = Property;
			PropInfo.PropertyValuePtr = PropertyData;
			PropInfo.DisplayName = FloatProp->GetDisplayNameText().ToString();
			PropInfo.PropertyPath = Prefix.IsEmpty() ? FloatProp->GetName() : FString::Printf(TEXT("%s.%s"), *Prefix, *FloatProp->GetName());
			const auto PropertyUnits = FloatProp->GetMetaData("Units");
			PropInfo.Unit = FUnitConversion::UnitFromString(*PropertyUnits);
			GetPropertyMinMax<float>(FloatProp, PropInfo.MinValue, PropInfo.MaxValue);
			PropInfo.DiscoveryOrder = DiscoveryOrder++;
			FloatProperties.Add(PropInfo);
		}
		else if (const FIntProperty* IntProp = CastField<FIntProperty>(Property))
		{
			FPropertyInformation PropInfo;
			PropInfo.Property = Property;
			PropInfo.PropertyValuePtr = PropertyData;
			PropInfo.DisplayName = IntProp->GetDisplayNameText().ToString();
			PropInfo.PropertyPath = Prefix.IsEmpty() ? IntProp->GetName() : FString::Printf(TEXT("%s.%s"), *Prefix, *IntProp->GetName());
			const auto PropertyUnits = IntProp->GetMetaData("Units");
			PropInfo.Unit = FUnitConversion::UnitFromString(*PropertyUnits);
			GetPropertyMinMax<int>(IntProp, PropInfo.MinValue, PropInfo.MaxValue);
			PropInfo.DiscoveryOrder = DiscoveryOrder++;
			IntProperties.Add(PropInfo);
		}
	}
}

void SForceFeedbackEffectDataEditor::ApplyVirtualDataToEffects()
{
	if (!VirtualEffectDataPtr || !EffectDataStruct || SimulationEffects.Num() == 0)
	{
		return;
	}

	// Apply virtual data to all simulation effects
	for (TWeakObjectPtr<UForceFeedbackEffectBase> SimEffect : SimulationEffects)
	{
		if (!SimEffect.IsValid())
		{
			continue;
		}

		// Find the EffectData property on the simulation effect
		const UClass* EffectClass = SimEffect->GetClass();
		const FProperty* SimEffectDataProp = nullptr;
		for (const UClass* Class = EffectClass; Class && Class != UObject::StaticClass(); Class = Class->GetSuperClass())
		{
			SimEffectDataProp = Class->FindPropertyByName(TEXT("EffectData"));
			if (SimEffectDataProp)
			{
				break;
			}
		}

		if (!SimEffectDataProp)
		{
			continue;
		}

		// Get pointer to the EffectData in the simulation effect
		void* SimEffectDataPtr = SimEffectDataProp->ContainerPtrToValuePtr<void>(SimEffect.Get());
		if (!SimEffectDataPtr)
		{
			continue;
		}

		// Copy virtual data to this simulation effect
		EffectDataStruct->CopyScriptStruct(SimEffectDataPtr, VirtualEffectDataPtr);

		// Update the effect with the virtual data
		if (SimEffect->IsInitialised)
		{
			SimEffect->UpdateEffect();
		}
	}
}

void SForceFeedbackEffectDataEditor::DestroySimulationEffects()
{
	for (TWeakObjectPtr<UForceFeedbackEffectBase> SimEffect : SimulationEffects)
	{
		if (SimEffect.IsValid())
		{
			// Force kill any running preview and fully destroy the haptic effect.
			SimEffect->StopEffect();
			SimEffect->DestroyEffect();
		}
	}
	SimulationEffects.Empty();
}

void SForceFeedbackEffectDataEditor::OnIntVectorValueChanged(FIntVector NewValue, const FPropertyInformation* PropertyInfo)
{
	if (!EffectPtr.IsValid() || !PropertyInfo || !PropertyInfo->PropertyValuePtr)
	{
		return;
	}

	// Clamp the value to min/max
	NewValue.X = FMath::Clamp(NewValue.X, PropertyInfo->MinValue, PropertyInfo->MaxValue);
	NewValue.Y = FMath::Clamp(NewValue.Y, PropertyInfo->MinValue, PropertyInfo->MaxValue);
	NewValue.Z = FMath::Clamp(NewValue.Z, PropertyInfo->MinValue, PropertyInfo->MaxValue);

	// Set the value in the virtual copy (not the actual effect data)
	if (FIntVector* VectorPtr = static_cast<FIntVector*>(PropertyInfo->PropertyValuePtr))
	{
		*VectorPtr = NewValue;
		bVirtualDirty = true;

		// Apply virtual data to all simulation effects
		ApplyVirtualDataToEffects();
	}
}

void SForceFeedbackEffectDataEditor::OnVectorValueChanged(FVector NewValue, const FPropertyInformation* PropertyInfo)
{
	if (!EffectPtr.IsValid() || !PropertyInfo || !PropertyInfo->PropertyValuePtr)
	{
		return;
	}

	// Clamp the value to min/max
	NewValue.X = FMath::Clamp(NewValue.X, PropertyInfo->MinValue, PropertyInfo->MaxValue);
	NewValue.Y = FMath::Clamp(NewValue.Y, PropertyInfo->MinValue, PropertyInfo->MaxValue);
	NewValue.Z = FMath::Clamp(NewValue.Z, PropertyInfo->MinValue, PropertyInfo->MaxValue);

	// Set the value in the virtual copy (not the actual effect data)
	if (FVector* VectorPtr = static_cast<FVector*>(PropertyInfo->PropertyValuePtr))
	{
		*VectorPtr = NewValue;
		bVirtualDirty = true;

		// Apply virtual data to all simulation effects
		ApplyVirtualDataToEffects();
	}
}

void SForceFeedbackEffectDataEditor::OnVector2DValueChanged(FVector2D NewValue, const FPropertyInformation* PropertyInfo)
{
	if (!EffectPtr.IsValid() || !PropertyInfo || !PropertyInfo->PropertyValuePtr)
	{
		return;
	}

	// Clamp the value to min/max
	NewValue.X = FMath::Clamp(NewValue.X, PropertyInfo->MinValue, PropertyInfo->MaxValue);
	NewValue.Y = FMath::Clamp(NewValue.Y, PropertyInfo->MinValue, PropertyInfo->MaxValue);

	// Set the value in the virtual copy (not the actual effect data)
	if (FVector2D* VectorPtr = static_cast<FVector2D*>(PropertyInfo->PropertyValuePtr))
	{
		*VectorPtr = NewValue;
		bVirtualDirty = true;

		// Apply virtual data to all simulation effects
		ApplyVirtualDataToEffects();
	}
}

#undef LOCTEXT_NAMESPACE
