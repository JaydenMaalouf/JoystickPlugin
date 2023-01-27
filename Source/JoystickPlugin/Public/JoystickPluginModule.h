// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Interfaces/JoystickPluginInterface.h"

class FJoystickInputDevice;

class JOYSTICKPLUGIN_API FJoystickPluginModule final : public IJoystickPlugin
{
public:
	virtual TSharedPtr<IInputDevice> CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;

	virtual void ShutdownModule() override;
	virtual void StartupModule() override;

	TSharedPtr<FJoystickInputDevice> GetJoystickInputDevice() const { return JoystickInputDevice; }

private:
	void* SdlDllHandle = nullptr;
	TSharedPtr<FJoystickInputDevice> JoystickInputDevice;
};
