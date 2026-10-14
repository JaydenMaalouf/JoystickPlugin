// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Customization/ForceFeedbackEffectDetails.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Editor.h"
#include "IDetailPropertyRow.h"
#include "PropertyHandle.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "Widgets/Input/SNumericEntryBox.h"

#define LOCTEXT_NAMESPACE "ForceFeedbackEffectDetails"

TSharedRef<IDetailCustomization> FForceFeedbackEffectDetails::MakeInstance()
{
	return MakeShareable(new FForceFeedbackEffectDetails);
}

void FForceFeedbackEffectDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
	DetailBuilder.GetObjectsBeingCustomized(ObjectsBeingCustomized);
	for (const TWeakObjectPtr<UObject>& Object : ObjectsBeingCustomized)
	{
		if (UForceFeedbackEffectBase* Effect = Cast<UForceFeedbackEffectBase>(Object.Get()))
		{
			Effects.Add(Effect);
		}
	}

	// Looping makes no difference to an effect whose first iteration never ends.
	const TSharedRef<IPropertyHandle> InfiniteIterationsHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UForceFeedbackEffectBase, InfiniteIterations), UForceFeedbackEffectBase::StaticClass());
	if (IDetailPropertyRow* InfiniteIterationsRow = DetailBuilder.EditDefaultProperty(InfiniteIterationsHandle))
	{
		InfiniteIterationsRow->IsEnabled(TAttribute<bool>::CreateSP(this, &FForceFeedbackEffectDetails::HasFiniteDuration));
	}

	IterationsHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UForceFeedbackEffectBase, Iterations), UForceFeedbackEffectBase::StaticClass());
	if (!IterationsHandle.IsValid() || !IterationsHandle->IsValidHandle())
	{
		return;
	}

	IDetailPropertyRow* IterationsRow = DetailBuilder.EditDefaultProperty(IterationsHandle);
	if (!IterationsRow)
	{
		return;
	}

	// The upper limit follows the effect's duration, which static ClampMax metadata can't do, so the value widget is replaced with one bound to it.
	// Combined with the InfiniteIterations edit condition rather than replacing it.
	IterationsRow->IsEnabled(TAttribute<bool>::CreateSP(this, &FForceFeedbackEffectDetails::HasFiniteDuration));

	IterationsRow->CustomWidget()
	             .NameContent()
		[
			IterationsHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SNumericEntryBox<int64>)
			.Font(IDetailLayoutBuilder::GetDetailFont())
			.AllowSpin(true)
			.MinValue(1)
			.MinSliderValue(1)
			.MaxValue(this, &FForceFeedbackEffectDetails::GetMaxIterations)
			.MaxSliderValue(this, &FForceFeedbackEffectDetails::GetMaxIterations)
			.Value(this, &FForceFeedbackEffectDetails::GetIterations)
			.OnValueChanged(this, &FForceFeedbackEffectDetails::OnIterationsChanged)
			.OnValueCommitted(this, &FForceFeedbackEffectDetails::OnIterationsCommitted)
			.OnBeginSliderMovement(this, &FForceFeedbackEffectDetails::OnBeginIterationsSliderMovement)
			.OnEndSliderMovement(this, &FForceFeedbackEffectDetails::OnEndIterationsSliderMovement)
		];
}

TOptional<int64> FForceFeedbackEffectDetails::GetIterations() const
{
	// Left unset when the selected effects disagree, which the entry box shows as multiple values.
	int64 Value = 0;
	if (IterationsHandle->GetValue(Value) == FPropertyAccess::Success)
	{
		return Value;
	}

	return TOptional<int64>();
}

TOptional<int64> FForceFeedbackEffectDetails::GetMaxIterations() const
{
	// The lowest limit across the selection, so the value is playable by every effect being edited.
	int64 MaxIterations = SDL_HAPTIC_INFINITY;
	for (const TWeakObjectPtr<UForceFeedbackEffectBase>& Effect : Effects)
	{
		if (Effect.IsValid())
		{
			MaxIterations = FMath::Min<int64>(MaxIterations, Effect->GetMaxIterations());
		}
	}

	return MaxIterations;
}

bool FForceFeedbackEffectDetails::HasFiniteDuration() const
{
	// An infinite duration never finishes its first iteration, so the count only matters if an effect can get past it.
	for (const TWeakObjectPtr<UForceFeedbackEffectBase>& Effect : Effects)
	{
		if (Effect.IsValid() && Effect->GetEffectDuration() != SDL_HAPTIC_INFINITY)
		{
			return true;
		}
	}

	return false;
}

void FForceFeedbackEffectDetails::OnIterationsChanged(const int64 NewValue) const
{
	IterationsHandle->SetValue(NewValue, EPropertyValueSetFlags::InteractiveChange);
}

void FForceFeedbackEffectDetails::OnIterationsCommitted(const int64 NewValue, ETextCommit::Type CommitType) const
{
	IterationsHandle->SetValue(NewValue);
}

void FForceFeedbackEffectDetails::OnBeginIterationsSliderMovement() const
{
	// Wraps the whole drag so it undoes as a single step.
	GEditor->BeginTransaction(LOCTEXT("SetIterations", "Set Iterations"));
}

void FForceFeedbackEffectDetails::OnEndIterationsSliderMovement(const int64 NewValue) const
{
	IterationsHandle->SetValue(NewValue);
	GEditor->EndTransaction();
}

#undef LOCTEXT_NAMESPACE
