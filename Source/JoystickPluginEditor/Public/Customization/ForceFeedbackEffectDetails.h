// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "IDetailCustomization.h"

class IPropertyHandle;
class UForceFeedbackEffectBase;

class FForceFeedbackEffectDetails : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

protected:
	TOptional<int64> GetIterations() const;
	TOptional<int64> GetMaxIterations() const;
	bool HasFiniteDuration() const;

	void OnIterationsChanged(int64 NewValue) const;
	void OnIterationsCommitted(int64 NewValue, ETextCommit::Type CommitType) const;
	void OnBeginIterationsSliderMovement() const;
	void OnEndIterationsSliderMovement(int64 NewValue) const;

	TSharedPtr<IPropertyHandle> IterationsHandle;
	TArray<TWeakObjectPtr<UForceFeedbackEffectBase>> Effects;
};
