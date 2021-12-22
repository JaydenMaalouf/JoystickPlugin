#pragma once

#include "Interfaces/JoystickPluginInterface.h"
#include "JoystickDeviceManager.h"
#include "JoystickHapticDeviceManager.h"
#include "JoystickInputDevice.h"

class FJoystickPlugin : public IJoystickPlugin
{

public:
	virtual TSharedPtr<class IInputDevice> CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;

	void ShutdownModule() override;

	void StartupModule() override;

	UJoystickDeviceManager* GetDeviceManager() const { return JoystickDeviceManager.Get(); }
	UJoystickHapticDeviceManager* GetHapticDeviceManager() const { return JoystickHapticDeviceManager.Get(); }

private:

	void* SdlDllHandle = nullptr;

	TSharedPtr<class FJoystickInputDevice> JoystickInputDevice;
	TSharedPtr<class UJoystickDeviceManager> JoystickDeviceManager;
	TSharedPtr<class UJoystickHapticDeviceManager> JoystickHapticDeviceManager;
};
