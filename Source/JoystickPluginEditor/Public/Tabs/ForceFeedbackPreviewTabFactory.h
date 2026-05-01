// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ForceFeedback/ForceFeedbackEffectPreviewPanel.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class FBlueprintEditor;

class FForceFeedbackPreviewTabFactory : public FWorkflowTabFactory
{
public:
	explicit FForceFeedbackPreviewTabFactory(const TSharedPtr<FBlueprintEditor>& InBlueprintEditor);

	virtual TSharedRef<SDockTab> SpawnTab(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<FBlueprintEditor> BlueprintEditorPtr;
};
