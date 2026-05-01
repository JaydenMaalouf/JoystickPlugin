// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/ForceFeedback/ForceFeedbackEffectPreviewPanel.h"

#include "BlueprintEditor.h"
#include "Editor.h"
#include "Data/PropertyInformation.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "Widgets/ForceFeedback/ForceFeedbackEffectDataEditor.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "ForceFeedbackEffectPreviewPanel"

void SForceFeedbackEffectPreviewPanel::Construct(const FArguments& InArgs)
{
	BlueprintEditorWeak = InArgs._BlueprintEditor;

	BindDelegates();

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(6.0f)
		[
			SNew(SButton)
			.ToolTipText_Lambda([this]()
			{
				if (!EffectPtr.IsValid())
				{
					return LOCTEXT("NoEffectTooltip", "Select a UForceFeedbackEffectBase to enable preview.");
				}

				if (EffectDataEditor.IsValid() && EffectDataEditor->IsSimulating())
				{
					return LOCTEXT("StopEffectTooltip", "Stop simulation and destroy all preview effect instances");
				}
				return LOCTEXT("PlayEffectTooltip", "Create effect instances for all FFB-capable devices and start simulation");
			})
			.IsEnabled_Lambda([this]()
			{
				return EffectPtr.IsValid();
			})
			.OnClicked_Lambda([this]()
			{
				if (EffectDataEditor.IsValid())
				{
					EffectDataEditor->CreateSimulationEffects();
				}
				return FReply::Handled();
			})
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
					SNew(SImage)
					.Image(this, &SForceFeedbackEffectPreviewPanel::GetButtonIcon)
				]

				+ SHorizontalBox::Slot()
				.Padding(6.f, 0.f)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text_Lambda([this]()
					{
						if (EffectDataEditor.IsValid() && EffectDataEditor->IsSimulating())
						{
							return LOCTEXT("StopEffect", "Stop Effect");
						}
						return LOCTEXT("PlayEffect", "Play Effect");
					})
				]
			]
		]
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.Padding(6.0f)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			[
				SAssignNew(PropertyRowsBox, SVerticalBox)
			]
		]
	];

	RefreshBlueprintCDO();
}

void SForceFeedbackEffectPreviewPanel::BindDelegates()
{
	if (GEditor)
	{
		BlueprintCompiledHandle =
			GEditor->OnBlueprintCompiled().AddSP(this, &SForceFeedbackEffectPreviewPanel::OnAnyBlueprintCompiled);
	}
}

void SForceFeedbackEffectPreviewPanel::UnbindDelegates()
{
	if (GEditor && BlueprintCompiledHandle.IsValid())
	{
		GEditor->OnBlueprintCompiled().Remove(BlueprintCompiledHandle);
		BlueprintCompiledHandle.Reset();
	}
}

void SForceFeedbackEffectPreviewPanel::RefreshBlueprintCDO()
{
	const TSharedPtr<FBlueprintEditor> BlueprintEditor = BlueprintEditorWeak.Pin();
	if (!BlueprintEditor.IsValid())
	{
		return;
	}

	const UBlueprint* Blueprint = BlueprintEditor->GetBlueprintObj();
	if (!Blueprint)
	{
		return;
	}

	const UClass* GeneratedClass = Blueprint->GeneratedClass;
	if (!GeneratedClass)
	{
		return;
	}

	UObject* CDO = GeneratedClass->GetDefaultObject();
	if (!CDO)
	{
	}

	UForceFeedbackEffectBase* ForceFeedbackEffectBase = Cast<UForceFeedbackEffectBase>(CDO);
	if (!ForceFeedbackEffectBase)
	{
		return;
	}

	SetEffect(ForceFeedbackEffectBase);
}

void SForceFeedbackEffectPreviewPanel::OnAnyBlueprintCompiled()
{
	RefreshBlueprintCDO();
}

void SForceFeedbackEffectPreviewPanel::RebuildPropertyRows() const
{
	if (!PropertyRowsBox.IsValid())
	{
		return;
	}

	PropertyRowsBox->ClearChildren();

	if (!EffectPtr.IsValid())
	{
		PropertyRowsBox->AddSlot()
		               .AutoHeight()
		               .Padding(2.0f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("NoSelectionBody", "Nothing selected. Select a Force Feedback Effect (UForceFeedbackEffectBase) in the editor to populate this panel."))
		];
		return;
	}

	if (!EffectDataEditor.IsValid())
	{
		PropertyRowsBox->AddSlot()
		               .AutoHeight()
		               .Padding(2.0f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("NoEditorWidget", "Unable to build preview UI for the selected effect."))
		];
		return;
	}

	// Helper structure to hold property information with discovery order for sorting
	struct FPropertyDisplayInfo
	{
		int32 DiscoveryOrder;
		TFunction<void()> AddWidgetFunc;

		bool operator<(const FPropertyDisplayInfo& Other) const
		{
			return DiscoveryOrder < Other.DiscoveryOrder;
		}
	};

	TArray<FPropertyDisplayInfo> PropertyDisplayInfos;

	// Collect all float properties
	for (const FPropertyInformation& PropInfo : EffectDataEditor->GetFloatProperties())
	{
		FPropertyDisplayInfo DisplayInfo;
		DisplayInfo.DiscoveryOrder = PropInfo.DiscoveryOrder;
		DisplayInfo.AddWidgetFunc = [this, PropInfo]()
		{
			const FString DisplayLabel = PropInfo.DisplayName.IsEmpty() ? PropInfo.PropertyPath : PropInfo.DisplayName;
			PropertyRowsBox->AddSlot()
			               .AutoHeight()
			               .Padding(2.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(0.45f)
				.VAlign(VAlign_Center)
				.Padding(0.0f, 0.0f, 8.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(DisplayLabel))
					.ToolTipText(FText::FromString(PropInfo.PropertyPath))
				]
				+ SHorizontalBox::Slot()
				.FillWidth(0.55f)
				.VAlign(VAlign_Center)
				[
					SNew(SBox)
					[
						EffectDataEditor->CreateValueWidget<FFloatProperty, float>(PropInfo)
					]
				]
			];
		};
		PropertyDisplayInfos.Add(DisplayInfo);
	}

	// Collect all int properties
	for (const FPropertyInformation& PropInfo : EffectDataEditor->GetIntProperties())
	{
		FPropertyDisplayInfo DisplayInfo;
		DisplayInfo.DiscoveryOrder = PropInfo.DiscoveryOrder;
		DisplayInfo.AddWidgetFunc = [this, PropInfo]()
		{
			const FString DisplayLabel = PropInfo.DisplayName.IsEmpty() ? PropInfo.PropertyPath : PropInfo.DisplayName;
			PropertyRowsBox->AddSlot()
			               .AutoHeight()
			               .Padding(2.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(0.45f)
				.VAlign(VAlign_Center)
				.Padding(0.0f, 0.0f, 8.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(DisplayLabel))
					.ToolTipText(FText::FromString(PropInfo.PropertyPath))
				]
				+ SHorizontalBox::Slot()
				.FillWidth(0.55f)
				.VAlign(VAlign_Center)
				[
					SNew(SBox)
					[
						EffectDataEditor->CreateValueWidget<FIntProperty, int>(PropInfo)
					]
				]
			];
		};
		PropertyDisplayInfos.Add(DisplayInfo);
	}

	// Collect all vector properties
	for (const FPropertyInformation& PropInfo : EffectDataEditor->GetVectorProperties())
	{
		FPropertyDisplayInfo DisplayInfo;
		DisplayInfo.DiscoveryOrder = PropInfo.DiscoveryOrder;
		DisplayInfo.AddWidgetFunc = [this, PropInfo]()
		{
			const FString DisplayLabel = PropInfo.DisplayName.IsEmpty() ? PropInfo.PropertyPath : PropInfo.DisplayName;
			PropertyRowsBox->AddSlot()
			               .AutoHeight()
			               .Padding(2.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(0.45f)
				.VAlign(VAlign_Center)
				.Padding(0.0f, 0.0f, 8.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(DisplayLabel))
					.ToolTipText(FText::FromString(PropInfo.PropertyPath))
				]
				+ SHorizontalBox::Slot()
				.FillWidth(0.55f)
				.VAlign(VAlign_Center)
				[
					SNew(SBox)
					[
						EffectDataEditor->CreateVectorWidget(PropInfo)
					]
				]
			];
		};
		PropertyDisplayInfos.Add(DisplayInfo);
	}

	// Collect all vector2D properties
	for (const FPropertyInformation& PropInfo : EffectDataEditor->GetVector2DProperties())
	{
		FPropertyDisplayInfo DisplayInfo;
		DisplayInfo.DiscoveryOrder = PropInfo.DiscoveryOrder;
		DisplayInfo.AddWidgetFunc = [this, PropInfo]()
		{
			const FString DisplayLabel = PropInfo.DisplayName.IsEmpty() ? PropInfo.PropertyPath : PropInfo.DisplayName;
			PropertyRowsBox->AddSlot()
			               .AutoHeight()
			               .Padding(2.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(0.45f)
				.VAlign(VAlign_Center)
				.Padding(0.0f, 0.0f, 8.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(DisplayLabel))
					.ToolTipText(FText::FromString(PropInfo.PropertyPath))
				]
				+ SHorizontalBox::Slot()
				.FillWidth(0.55f)
				.VAlign(VAlign_Center)
				[
					SNew(SBox)
					[
						EffectDataEditor->CreateVector2DWidget(PropInfo)
					]
				]
			];
		};
		PropertyDisplayInfos.Add(DisplayInfo);
	}

	// Collect all int vector properties
	for (const FPropertyInformation& PropInfo : EffectDataEditor->GetIntVectorProperties())
	{
		FPropertyDisplayInfo DisplayInfo;
		DisplayInfo.DiscoveryOrder = PropInfo.DiscoveryOrder;
		DisplayInfo.AddWidgetFunc = [this, PropInfo]()
		{
			const FString DisplayLabel = PropInfo.DisplayName.IsEmpty() ? PropInfo.PropertyPath : PropInfo.DisplayName;
			PropertyRowsBox->AddSlot()
			               .AutoHeight()
			               .Padding(2.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(0.45f)
				.VAlign(VAlign_Center)
				.Padding(0.0f, 0.0f, 8.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(DisplayLabel))
					.ToolTipText(FText::FromString(PropInfo.PropertyPath))
				]
				+ SHorizontalBox::Slot()
				.FillWidth(0.55f)
				.VAlign(VAlign_Center)
				[
					SNew(SBox)
					[
						EffectDataEditor->CreateIntVectorWidget(PropInfo)
					]
				]
			];
		};
		PropertyDisplayInfos.Add(DisplayInfo);
	}

	// Sort by discovery order
	PropertyDisplayInfos.Sort();

	// Add widgets in sorted order
	for (const FPropertyDisplayInfo& DisplayInfo : PropertyDisplayInfos)
	{
		DisplayInfo.AddWidgetFunc();
	}
}

void SForceFeedbackEffectPreviewPanel::SetEffect(UForceFeedbackEffectBase* NewEffect)
{
	EffectPtr = NewEffect;
	EffectDataEditor.Reset();

	if (EffectPtr.IsValid())
	{
		EffectDataEditor = SNew(SForceFeedbackEffectDataEditor)
			.EffectPtr(EffectPtr);
	}

	RebuildPropertyRows();
}

const FSlateBrush* SForceFeedbackEffectPreviewPanel::GetButtonIcon() const
{
	static const FSlateIcon StopIcon(FAppStyle::GetAppStyleSetName(), "Icons.Toolbar.Stop");
	static const FSlateIcon StartIcon(FAppStyle::GetAppStyleSetName(), "Icons.Toolbar.Play");

	if (EffectDataEditor.IsValid() && EffectDataEditor->IsSimulating())
	{
		return StopIcon.GetIcon();
	}

	return StartIcon.GetIcon();
}

#undef LOCTEXT_NAMESPACE
