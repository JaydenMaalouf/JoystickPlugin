#pragma once

#include "Interfaces/JoystickPluginInterface.h"
#include "JoystickDeviceManager.h"
#include "JoystickHapticDeviceManager.h"
#include "JoystickInputDevice.h"

DECLARE_LOG_CATEGORY_EXTERN(LogJoystickPlugin, Log, All);

class FJoystickPlugin : public IJoystickPlugin
{

public:
	virtual TSharedPtr<class IInputDevice> CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;

	void ShutdownModule() override;

	void StartupModule() override;
	
	TSharedPtr<FJoystickInputDevice> GetJoystickInputDevice() const { return JoystickInputDevice; }

private:

	void* SdlDllHandle = nullptr;
	
	TSharedPtr<FJoystickInputDevice> JoystickInputDevice;
};
