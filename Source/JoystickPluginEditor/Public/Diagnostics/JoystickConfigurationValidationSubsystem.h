// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "EditorSubsystem.h"

#include "JoystickConfigurationValidationSubsystem.generated.h"

class SNotificationItem;
class SWindow;
class UJoystickConfigurationRule;
struct FJoystickInstanceId;
enum class ECheckBoxState : uint8;

UCLASS()
class JOYSTICKPLUGINEDITOR_API UJoystickConfigurationValidationSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	// UEditorSubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void EvaluateRules();

private:
	void CreateRules();
	void BindEvents();
	void UnbindEvents();

	bool IsMainFrameReady() const;
	void OnMainFrameCreated(TSharedPtr<SWindow> RootWindow, bool IsRunningStartupDialog);
	void OnJoystickConnectionChanged(const FJoystickInstanceId& InstanceId);
	void OnObjectPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent);
	bool IsWatchedObject(const UObject& Object) const;

	void EvaluateRule(const UJoystickConfigurationRule& Rule);
	bool CanShowNotification(const FName RuleId) const;
	bool IsNotificationActive(const FName RuleId) const;
	void ShowNotification(const UJoystickConfigurationRule& Rule);
	void HideNotification(const FName RuleId);

	const UJoystickConfigurationRule* FindRule(const FName RuleId) const;
	void ApplyFix(const FName RuleId);
	void OnNeverShowAgainChanged(const ECheckBoxState NewState, const FName RuleId);
	void DismissRule(const FName RuleId);
	void DismissRuleIndefinitely(const FName RuleId);
	void OpenSettings(const FName CategoryName, const FName SectionName) const;

	UPROPERTY()
	TArray<UJoystickConfigurationRule*> Rules;

	TMap<FName, TWeakPtr<SNotificationItem>> ActiveNotifications;

	// Dismissed for this session until the issue resolves, after which it can show again.
	TSet<FName> SessionDismissedRules;

	TSet<FName> NeverShowAgainRules;

	FDelegateHandle JoystickPluggedInHandle;
	FDelegateHandle JoystickUnpluggedHandle;
	FDelegateHandle ObjectPropertyChangedHandle;
	FDelegateHandle MainFrameCreatedHandle;
};
