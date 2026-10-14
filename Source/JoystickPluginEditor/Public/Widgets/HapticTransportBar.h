// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"

// The Haptic Effect Editor's Play/Stop/Loop buttons. Holds no playback state of its own.
class SHapticTransportBar : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticTransportBar)
			: _HasDevice(false)
			  , _IsPlaying(false)
			  , _IsLooping(false)
		{
		}

		SLATE_ATTRIBUTE(bool, HasDevice)
		SLATE_ATTRIBUTE(bool, IsPlaying)
		SLATE_ATTRIBUTE(bool, IsLooping)
		SLATE_EVENT(FOnClicked, OnPlayClicked)
		SLATE_EVENT(FOnClicked, OnStopClicked)
		SLATE_EVENT(FOnClicked, OnLoopClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TAttribute<bool> HasDevice;
	TAttribute<bool> IsPlaying;
	TAttribute<bool> IsLooping;
};
