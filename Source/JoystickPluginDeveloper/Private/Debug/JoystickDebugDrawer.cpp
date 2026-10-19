// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Debug/JoystickDebugDrawer.h"

#include "Debug/DebugDrawService.h"
#include "Debug/JoystickDebugCanvas.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Misc/CoreDelegates.h"

void FJoystickDebugDrawer::Register()
{
	if (IsValid(GEngine) && GEngine->IsInitialized())
	{
		RegisterDebugDraw();
		return;
	}

	PostEngineInitHandle = FCoreDelegates::OnPostEngineInit.AddRaw(this, &FJoystickDebugDrawer::RegisterDebugDraw);
}

void FJoystickDebugDrawer::Unregister()
{
	FCoreDelegates::OnPostEngineInit.Remove(PostEngineInitHandle);
	PostEngineInitHandle.Reset();

	if (DebugDrawHandle.IsValid())
	{
		UDebugDrawService::Unregister(DebugDrawHandle);
		DebugDrawHandle.Reset();
	}

	Overlays.Empty();
}

void FJoystickDebugDrawer::AddOverlay(TUniquePtr<FJoystickDebugOverlay> Overlay)
{
	Overlays.Add(MoveTemp(Overlay));
}

void FJoystickDebugDrawer::RegisterDebugDraw()
{
	DebugDrawHandle = UDebugDrawService::Register(TEXT("Game"), FDebugDrawDelegate::CreateRaw(this, &FJoystickDebugDrawer::Draw));
}

void FJoystickDebugDrawer::Draw(UCanvas* Canvas, APlayerController* PlayerController)
{
	if (IsValid(Canvas) == false || IsValid(PlayerController) == false)
	{
		return;
	}

	const UWorld* World = PlayerController->GetWorld();
	if (IsValid(World) == false || World->GetFirstPlayerController() != PlayerController)
	{
		return;
	}

	float Y = FJoystickDebugCanvas::OriginY;
	for (const TUniquePtr<FJoystickDebugOverlay>& Overlay : Overlays)
	{
		if (Overlay->IsEnabled())
		{
			Y = Overlay->Draw(Canvas, World, Y) + FJoystickDebugCanvas::OverlaySpacing;
		}
	}
}
