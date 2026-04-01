// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Tabs/ForceFeedbackPreviewTabFactory.h"

#include "BlueprintEditor.h"
#include "JoystickPluginEditorTabIds.h"
#include "Styling/AppStyle.h"
#include "Widgets/ForceFeedback/ForceFeedbackEffectPreviewPanel.h"

#define LOCTEXT_NAMESPACE "ForceFeedbackPreviewTabFactory"

FForceFeedbackPreviewTabFactory::FForceFeedbackPreviewTabFactory(const TSharedPtr<FBlueprintEditor>& InBlueprintEditor)
	: FWorkflowTabFactory(JoystickPluginEditorTabIds::ForceFeedbackPreview, InBlueprintEditor)
	  , BlueprintEditorPtr(InBlueprintEditor)
{
	TabLabel = LOCTEXT("ForceFeedbackPreviewTabLabel", "Force Feedback Preview");
	ViewMenuDescription = LOCTEXT("ForceFeedbackPreviewTabMenuDesc", "Force Feedback Preview");
	ViewMenuTooltip = LOCTEXT("ForceFeedbackPreviewTabMenuTooltip", "Preview and play the currently edited Force Feedback effect.");
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Play");
	TabRole = PanelTab;
	bIsSingleton = true;
}

TSharedRef<SDockTab> FForceFeedbackPreviewTabFactory::SpawnTab(const FWorkflowTabSpawnInfo& Info) const
{
	return SNew(SDockTab)
		.Label(LOCTEXT("ForceFeedbackPreviewTab", "Force Feedback Preview"))
		[
			SNew(SForceFeedbackEffectPreviewPanel)
			.BlueprintEditor(BlueprintEditorPtr)
		];
}

#undef LOCTEXT_NAMESPACE
