// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Customization/JoystickDirectionCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"

TSharedRef<IPropertyTypeCustomization> FJoystickDirectionCustomization::MakeInstance()
{
	return MakeShareable(new FJoystickDirectionCustomization);
}

void FJoystickDirectionCustomization::GetSortedChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, TArray<TSharedRef<IPropertyHandle>>& OutChildren)
{
	static const FName X("X");
	static const FName Y("Y");
	static const FName Z("Z");

	TSharedPtr<IPropertyHandle> VectorChildren[3];

	uint32 NumChildren;
	StructPropertyHandle->GetNumChildren(NumChildren);

	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
	{
		const TSharedRef<IPropertyHandle> ChildHandle = StructPropertyHandle->GetChildHandle(ChildIndex).ToSharedRef();
		const FName PropertyName = ChildHandle->GetProperty()->GetFName();

		if (PropertyName == X)
		{
			VectorChildren[0] = ChildHandle;
		}
		else if (PropertyName == Y)
		{
			VectorChildren[1] = ChildHandle;
		}
		else
		{
			check(PropertyName == Z);
			VectorChildren[2] = ChildHandle;
		}
	}

	uint8 EnumValue = 0;
	if (DirectionTypeHandle->IsValidHandle() && DirectionTypeHandle->GetValue(EnumValue) == FPropertyAccess::Success)
	{
		switch (EnumValue)
		{
		case 0:
			OutChildren.Add(VectorChildren[0].ToSharedRef());
			break;
		case 1:
			OutChildren.Add(VectorChildren[0].ToSharedRef());
			OutChildren.Add(VectorChildren[1].ToSharedRef());
			OutChildren.Add(VectorChildren[2].ToSharedRef());
			break;
		case 2:
			OutChildren.Add(VectorChildren[0].ToSharedRef());
			OutChildren.Add(VectorChildren[1].ToSharedRef());
			break;
		default: break;
		}
	}
}

void FJoystickDirectionCustomization::CustomizeHeader(const TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	DirectionTypeHandle = PropertyHandle->GetChildHandle("DirectionType");
	SortedChildHandles.Empty();
	const TSharedRef<IPropertyHandle> DirectionHandle = PropertyHandle->GetChildHandle("Direction").ToSharedRef();
	GetSortedChildren(DirectionHandle, SortedChildHandles);

	HeaderRow
		.NameContent()[PropertyHandle->CreatePropertyNameWidget()]
		.ValueContent()[PropertyHandle->CreatePropertyValueWidget()];
}

void FJoystickDirectionCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	ChildBuilder.AddProperty(DirectionTypeHandle.ToSharedRef());
	for (int32 ChildIndex = 0; ChildIndex < SortedChildHandles.Num(); ++ChildIndex)
	{
		const TSharedRef<IPropertyHandle> ChildHandle = SortedChildHandles[ChildIndex];

		// Add the individual properties as children as well so the vector can be expanded for more room
		ChildBuilder.AddProperty(ChildHandle);
	}

	const TWeakPtr<IPropertyUtilities> WeakPropUtils = CustomizationUtils.GetPropertyUtilities();
	if (DirectionTypeHandle.IsValid())
	{
		DirectionTypeHandle->SetOnPropertyValueChanged(
			FSimpleDelegate::CreateLambda([WeakPropUtils]()
			{
				if (const TSharedPtr<IPropertyUtilities> Pinned = WeakPropUtils.Pin())
				{
					Pinned->ForceRefresh();
				}
			})
		);
	}
}
