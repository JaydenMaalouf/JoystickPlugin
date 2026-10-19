// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Debug/JoystickDebugOverlay.h"

class APlayerController;
class UCanvas;

// Draws every enabled overlay into the game viewport, one under the other.
class FJoystickDebugDrawer
{
public:
	void Register();
	void Unregister();
	void AddOverlay(TUniquePtr<FJoystickDebugOverlay> Overlay);

private:
	void RegisterDebugDraw();
	void Draw(UCanvas* Canvas, APlayerController* PlayerController);

	TArray<TUniquePtr<FJoystickDebugOverlay>> Overlays;
	FDelegateHandle PostEngineInitHandle;
	FDelegateHandle DebugDrawHandle;
};
