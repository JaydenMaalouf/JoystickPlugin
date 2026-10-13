// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Dialogs/HapticEffectAssetTypeDialog.h"

#include "Editor.h"
#include "SPrimaryButton.h"
#include "Engine/Blueprint.h"
#include "ForceFeedback/Data/ForceFeedbackEffectGroup.h"
#include "Styling/AppStyle.h"
#include "Styling/SlateIconFinder.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SWindow.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"

#define LOCTEXT_NAMESPACE "HapticEffectEditor"

TOptional<EHapticEffectAssetType> FHapticEffectAssetTypeDialog::Open()
{
	struct FAssetTypeOption
	{
		EHapticEffectAssetType AssetType;
		FText Name;
		FText Description;
		const FSlateBrush* Icon;
	};

	const TArray<TSharedPtr<FAssetTypeOption>> Options = {
		MakeShared<FAssetTypeOption>(FAssetTypeOption{
			EHapticEffectAssetType::BlueprintClass,
			LOCTEXT("SaveAsBlueprintClass", "Effect Blueprint Class"),
			LOCTEXT("SaveAsBlueprintClassDescription", "A Blueprint subclass of this effect type with these values as its defaults. Use it as a force feedback component's Effect Type."),
			FSlateIconFinder::FindIconBrushForClass(UBlueprint::StaticClass())
		}),
		MakeShared<FAssetTypeOption>(FAssetTypeOption{
			EHapticEffectAssetType::EffectGroup,
			LOCTEXT("SaveAsEffectGroup", "Effect Group Data Asset"),
			LOCTEXT("SaveAsEffectGroupDescription", "A Force Feedback Effect Group holding this effect as its only entry. More effects can be added to the group afterwards."),
			FSlateIconFinder::FindIconBrushForClass(UForceFeedbackEffectGroup::StaticClass())
		})
	};

	// The window is modal, so these locals outlive every delegate that captures them by reference.
	TSharedPtr<FAssetTypeOption> SelectedOption = Options[0];
	bool bConfirmed = false;

	const TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(LOCTEXT("PickAssetTypeTitle", "Pick Asset Type For Effect"))
		.ClientSize(FVector2D(440.0f, 240.0f))
		.SizingRule(ESizingRule::FixedSize)
		.SupportsMinimize(false)
		.SupportsMaximize(false);

	const auto Confirm = [&bConfirmed, &Window]
	{
		bConfirmed = true;
		Window->RequestDestroyWindow();
	};

	TSharedPtr<SListView<TSharedPtr<FAssetTypeOption>>> ListView;

	Window->SetContent(
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("Brushes.Panel"))
		.Padding(8.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("Brushes.Recessed"))
				[
					SAssignNew(ListView, SListView<TSharedPtr<FAssetTypeOption>>)
					.ListItemsSource(&Options)
					.SelectionMode(ESelectionMode::Single)
					.OnGenerateRow_Lambda([](const TSharedPtr<FAssetTypeOption>& Option, const TSharedRef<STableViewBase>& OwnerTable) -> TSharedRef<ITableRow>
					{
						return SNew(STableRow<TSharedPtr<FAssetTypeOption>>, OwnerTable)
							.Padding(FMargin(6.0f, 6.0f))
							[
								SNew(SHorizontalBox)
								+ SHorizontalBox::Slot()
								.AutoWidth()
								.VAlign(VAlign_Top)
								.Padding(0, 2, 8, 0)
								[
									SNew(SImage)
									.Image(Option->Icon)
								]
								+ SHorizontalBox::Slot()
								.FillWidth(1.0f)
								[
									SNew(SVerticalBox)
									+ SVerticalBox::Slot()
									.AutoHeight()
									[
										SNew(STextBlock)
										.Text(Option->Name)
										.TextStyle(FAppStyle::Get(), "NormalText")
									]
									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(0, 2, 0, 0)
									[
										SNew(STextBlock)
										.Text(Option->Description)
										.TextStyle(FAppStyle::Get(), "SmallText")
										.ColorAndOpacity(FSlateColor::UseSubduedForeground())
										.AutoWrapText(true)
									]
								]
							];
					})
					.OnSelectionChanged_Lambda([&SelectedOption](const TSharedPtr<FAssetTypeOption>& Option, ESelectInfo::Type)
					{
						SelectedOption = Option;
					})
					.OnMouseButtonDoubleClick_Lambda([&SelectedOption, &Confirm](const TSharedPtr<FAssetTypeOption>& Option)
					{
						SelectedOption = Option;
						Confirm();
					})
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Right)
			.Padding(0, 8, 0, 0)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(0, 0, 8, 0)
				[
					SNew(SPrimaryButton)
					.Text(LOCTEXT("PickAssetTypeSelect", "Select"))
					.IsEnabled_Lambda([&SelectedOption] { return SelectedOption.IsValid(); })
					.OnClicked_Lambda([&Confirm]
					{
						Confirm();
						return FReply::Handled();
					})
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("PickAssetTypeCancel", "Cancel"))
					.OnClicked_Lambda([&Window]
					{
						Window->RequestDestroyWindow();
						return FReply::Handled();
					})
				]
			]
		]);

	ListView->SetSelection(SelectedOption);

	GEditor->EditorAddModalWindow(Window);

	if (bConfirmed && SelectedOption.IsValid())
	{
		return SelectedOption->AssetType;
	}

	return TOptional<EHapticEffectAssetType>();
}

#undef LOCTEXT_NAMESPACE
