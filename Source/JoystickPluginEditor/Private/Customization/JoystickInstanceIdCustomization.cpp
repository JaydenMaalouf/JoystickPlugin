#include "Customization/JoystickInstanceIdCustomization.h"


#if WITH_EDITOR

#include "Data/JoystickInstanceId.h"
#include "DetailWidgetRow.h"
#include "DetailLayoutBuilder.h"

void FJoystickInstanceIdCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	ValueProp = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FJoystickInstanceId, Value));

	FText ValueDisplay;
	ValueProp->GetValueAsDisplayText(ValueDisplay);

	HeaderRow
		.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget(ValueProp->GetPropertyDisplayName())
		]
		.ValueContent().MinDesiredWidth(500)
		[
			SNew(STextBlock)
			.Text(ValueDisplay)
			.Font(IDetailLayoutBuilder::GetDetailFont())
		];
}

void FJoystickInstanceIdCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
}

#endif
