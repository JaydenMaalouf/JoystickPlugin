// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Diagnostics/JoystickConfigurationValidationSubsystem.h"

#include "Diagnostics/JoystickConfigurationNotificationSettings.h"
#include "Diagnostics/JoystickConfigurationRule.h"
#include "Engine/Engine.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Interfaces/IMainFrameModule.h"
#include "ISettingsModule.h"
#include "JoystickSubsystem.h"
#include "Modules/ModuleManager.h"
#include "UObject/UObjectHash.h"
#include "Widgets/ConfigurationNotification/ConfigurationNotification.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Widgets/SWindow.h"

void UJoystickConfigurationValidationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CreateRules();
	BindEvents();

	if (IsMainFrameReady())
	{
		EvaluateRules();
		return;
	}

	IMainFrameModule* MainFrameModule = FModuleManager::LoadModulePtr<IMainFrameModule>("MainFrame");
	if (MainFrameModule == nullptr)
	{
		return;
	}

	MainFrameCreatedHandle = MainFrameModule->OnMainFrameCreationFinished().AddUObject(this, &UJoystickConfigurationValidationSubsystem::OnMainFrameCreated);
}

void UJoystickConfigurationValidationSubsystem::Deinitialize()
{
	UnbindEvents();

	TArray<FName> RuleIds;
	ActiveNotifications.GenerateKeyArray(RuleIds);
	for (const FName RuleId : RuleIds)
	{
		HideNotification(RuleId);
	}

	Rules.Empty();

	Super::Deinitialize();
}

void UJoystickConfigurationValidationSubsystem::EvaluateRules()
{
	if (IsMainFrameReady() == false)
	{
		return;
	}

	for (const UJoystickConfigurationRule* Rule : Rules)
	{
		if (IsValid(Rule) == false)
		{
			continue;
		}

		EvaluateRule(*Rule);
	}
}

void UJoystickConfigurationValidationSubsystem::CreateRules()
{
	TArray<UClass*> RuleClasses;
	GetDerivedClasses(UJoystickConfigurationRule::StaticClass(), RuleClasses);

	for (const UClass* RuleClass : RuleClasses)
	{
		if (RuleClass->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists))
		{
			continue;
		}

		Rules.Add(NewObject<UJoystickConfigurationRule>(this, RuleClass));
	}
}

void UJoystickConfigurationValidationSubsystem::BindEvents()
{
	ObjectPropertyChangedHandle = FCoreUObjectDelegates::OnObjectPropertyChanged.AddUObject(this, &UJoystickConfigurationValidationSubsystem::OnObjectPropertyChanged);

	if (IsValid(GEngine) == false)
	{
		return;
	}

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (IsValid(JoystickSubsystem) == false)
	{
		return;
	}

	JoystickPluggedInHandle = JoystickSubsystem->Internal_JoystickPluggedInDelegate.AddUObject(this, &UJoystickConfigurationValidationSubsystem::OnJoystickConnectionChanged);
	JoystickUnpluggedHandle = JoystickSubsystem->Internal_JoystickUnpluggedDelegate.AddUObject(this, &UJoystickConfigurationValidationSubsystem::OnJoystickConnectionChanged);
}

void UJoystickConfigurationValidationSubsystem::UnbindEvents()
{
	FCoreUObjectDelegates::OnObjectPropertyChanged.Remove(ObjectPropertyChangedHandle);

	IMainFrameModule* MainFrameModule = FModuleManager::GetModulePtr<IMainFrameModule>("MainFrame");
	if (MainFrameModule != nullptr)
	{
		MainFrameModule->OnMainFrameCreationFinished().Remove(MainFrameCreatedHandle);
	}

	if (IsValid(GEngine) == false)
	{
		return;
	}

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (IsValid(JoystickSubsystem) == false)
	{
		return;
	}

	JoystickSubsystem->Internal_JoystickPluggedInDelegate.Remove(JoystickPluggedInHandle);
	JoystickSubsystem->Internal_JoystickUnpluggedDelegate.Remove(JoystickUnpluggedHandle);
}

bool UJoystickConfigurationValidationSubsystem::IsMainFrameReady() const
{
	const IMainFrameModule* MainFrameModule = FModuleManager::GetModulePtr<IMainFrameModule>("MainFrame");
	if (MainFrameModule == nullptr)
	{
		return false;
	}

	const TSharedPtr<SWindow> MainFrameWindow = MainFrameModule->GetParentWindow();
	if (MainFrameWindow.IsValid() == false)
	{
		return false;
	}

	return MainFrameWindow->IsVisible() && MainFrameWindow->IsWindowMinimized() == false;
}

void UJoystickConfigurationValidationSubsystem::OnMainFrameCreated(TSharedPtr<SWindow> RootWindow, bool IsRunningStartupDialog)
{
	EvaluateRules();
}

void UJoystickConfigurationValidationSubsystem::OnJoystickConnectionChanged(const FJoystickInstanceId& InstanceId)
{
	EvaluateRules();
}

void UJoystickConfigurationValidationSubsystem::OnObjectPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent)
{
	if (IsValid(Object) == false)
	{
		return;
	}

	if (IsWatchedObject(*Object) == false)
	{
		return;
	}

	EvaluateRules();
}

bool UJoystickConfigurationValidationSubsystem::IsWatchedObject(const UObject& Object) const
{
	if (Object.IsA<UJoystickConfigurationNotificationSettings>())
	{
		return true;
	}

	return Rules.ContainsByPredicate([&Object](const UJoystickConfigurationRule* Rule)
	{
		return IsValid(Rule) && Rule->IsAffectedBy(Object);
	});
}

void UJoystickConfigurationValidationSubsystem::EvaluateRule(const UJoystickConfigurationRule& Rule)
{
	const FName RuleId = Rule.GetRuleId();
	if (RuleId == NAME_None)
	{
		return;
	}

	if (Rule.HasIssue() == false)
	{
		SessionDismissedRules.Remove(RuleId);
		HideNotification(RuleId);
		return;
	}

	if (CanShowNotification(RuleId) == false)
	{
		HideNotification(RuleId);
		return;
	}

	if (IsNotificationActive(RuleId))
	{
		return;
	}

	ShowNotification(Rule);
}

bool UJoystickConfigurationValidationSubsystem::CanShowNotification(const FName RuleId) const
{
	if (SessionDismissedRules.Contains(RuleId))
	{
		return false;
	}

	const UJoystickConfigurationNotificationSettings* NotificationSettings = GetDefault<UJoystickConfigurationNotificationSettings>();
	if (IsValid(NotificationSettings) == false)
	{
		return false;
	}

	return NotificationSettings->EnableNotifications && NotificationSettings->IsRuleDismissed(RuleId) == false;
}

bool UJoystickConfigurationValidationSubsystem::IsNotificationActive(const FName RuleId) const
{
	const TWeakPtr<SNotificationItem>* Notification = ActiveNotifications.Find(RuleId);
	if (Notification == nullptr)
	{
		return false;
	}

	return Notification->IsValid();
}

void UJoystickConfigurationValidationSubsystem::ShowNotification(const UJoystickConfigurationRule& Rule)
{
	const FName RuleId = Rule.GetRuleId();

	FSimpleDelegate OnApplyFix;
	FText FixLabel;
	if (Rule.GetFixLabel(FixLabel))
	{
		OnApplyFix = FSimpleDelegate::CreateUObject(this, &UJoystickConfigurationValidationSubsystem::ApplyFix, RuleId);
	}

	FSimpleDelegate OnOpenSettings;
	FName CategoryName;
	FName SectionName;
	if (Rule.GetSettingsLocation(CategoryName, SectionName))
	{
		OnOpenSettings = FSimpleDelegate::CreateUObject(this, &UJoystickConfigurationValidationSubsystem::OpenSettings, CategoryName, SectionName);
	}

	const TSharedRef<SConfigurationNotification> Content = SNew(SConfigurationNotification)
		.Title(Rule.GetText())
		.Body(Rule.GetSubText())
		.FixLabel(FixLabel)
		.OnApplyFix(OnApplyFix)
		.OnOpenSettings(OnOpenSettings)
		.OnDismiss(FSimpleDelegate::CreateUObject(this, &UJoystickConfigurationValidationSubsystem::DismissRule, RuleId))
		.OnNeverShowAgainChanged(FOnCheckStateChanged::CreateUObject(this, &UJoystickConfigurationValidationSubsystem::OnNeverShowAgainChanged, RuleId));

	FNotificationInfo Info(Content);
	Info.bFireAndForget = false;
	Info.WidthOverride = 480.0f;
	Info.FadeInDuration = 1.0f;
	Info.FadeOutDuration = 0.5f;

	const TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(Info);
	if (Notification.IsValid() == false)
	{
		return;
	}

	ActiveNotifications.Add(RuleId, Notification);
}

void UJoystickConfigurationValidationSubsystem::HideNotification(const FName RuleId)
{
	NeverShowAgainRules.Remove(RuleId);

	TWeakPtr<SNotificationItem> Notification;
	if (ActiveNotifications.RemoveAndCopyValue(RuleId, Notification) == false)
	{
		return;
	}

	const TSharedPtr<SNotificationItem> PinnedNotification = Notification.Pin();
	if (PinnedNotification.IsValid() == false)
	{
		return;
	}

	PinnedNotification->Fadeout();
}

const UJoystickConfigurationRule* UJoystickConfigurationValidationSubsystem::FindRule(const FName RuleId) const
{
	UJoystickConfigurationRule* const* Rule = Rules.FindByPredicate([RuleId](const UJoystickConfigurationRule* Candidate)
	{
		return IsValid(Candidate) && Candidate->GetRuleId() == RuleId;
	});

	if (Rule == nullptr)
	{
		return nullptr;
	}

	return *Rule;
}

void UJoystickConfigurationValidationSubsystem::ApplyFix(const FName RuleId)
{
	const UJoystickConfigurationRule* Rule = FindRule(RuleId);
	if (IsValid(Rule) == false)
	{
		return;
	}

	Rule->ApplyFix();
	EvaluateRules();

	FName CategoryName;
	FName SectionName;
	if (Rule->GetSettingsLocation(CategoryName, SectionName) == false)
	{
		return;
	}

	OpenSettings(CategoryName, SectionName);
}

void UJoystickConfigurationValidationSubsystem::OnNeverShowAgainChanged(const ECheckBoxState NewState, const FName RuleId)
{
	if (NewState == ECheckBoxState::Checked)
	{
		NeverShowAgainRules.Add(RuleId);
		return;
	}

	NeverShowAgainRules.Remove(RuleId);
}

void UJoystickConfigurationValidationSubsystem::DismissRule(const FName RuleId)
{
	if (NeverShowAgainRules.Contains(RuleId))
	{
		DismissRuleIndefinitely(RuleId);
		return;
	}

	SessionDismissedRules.Add(RuleId);
	HideNotification(RuleId);
}

void UJoystickConfigurationValidationSubsystem::DismissRuleIndefinitely(const FName RuleId)
{
	UJoystickConfigurationNotificationSettings* NotificationSettings = GetMutableDefault<UJoystickConfigurationNotificationSettings>();
	if (IsValid(NotificationSettings) == false)
	{
		return;
	}

	NotificationSettings->DismissRule(RuleId);
	HideNotification(RuleId);
}

void UJoystickConfigurationValidationSubsystem::OpenSettings(const FName CategoryName, const FName SectionName) const
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule == nullptr)
	{
		return;
	}

	SettingsModule->ShowViewer("Project", CategoryName, SectionName);
}
