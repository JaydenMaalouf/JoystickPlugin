// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Delegates/Delegate.h"
#include "Modules/ModuleInterface.h"

class FBlueprintEditor;
class FLayoutExtender;
class FWorkflowAllowedTabSet;

class FJoystickPluginEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterMenus() const;
	void RegisterPropertyLayout() const;
	void RegisterSettings() const;
	void UnregisterSettings() const;

	void RegisterBlueprintEditorTabs(FWorkflowAllowedTabSet& TabFactories, FName ModeName, TSharedPtr<FBlueprintEditor> BlueprintEditor) const;
	void RegisterBlueprintEditorLayout(FLayoutExtender& Extender) const;

	FDelegateHandle BlueprintTabsHandle;
	FDelegateHandle BlueprintLayoutHandle;
};
