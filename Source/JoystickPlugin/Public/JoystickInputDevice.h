#pragma once

#include "IInputDevice.h"
#include "Data/JoystickDeviceData.h"
#include "Data/JoystickInfo.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "Interfaces/JoystickEventInterface.h"

struct FDeviceInfoSDL;
class FDeviceSDL;

class FJoystickInputDevice : public IInputDevice, public IJoystickEventInterface
{
	
public:

	FJoystickInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);
	virtual ~FJoystickInputDevice() override;
		
	virtual void Tick(float DeltaTime) override;
	virtual void SendControllerEvents() override;
	virtual void SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override;
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues& Values) override;

	void IgnoreGameControllers(bool bIgnore) const;
	void ReinitialiseJoystickData(int32 DeviceId);

	virtual void JoystickPluggedIn(const FDeviceInfoSDL &Device) override;
	virtual void JoystickUnplugged(int32 DeviceId) override;
	virtual void JoystickButton(int32 DeviceId, int32 Button, bool Pressed) override;
	virtual void JoystickAxis(int32 DeviceId, int32 Axis, float Value) override;
	virtual void JoystickHat(int32 DeviceId, int32 Hat, EJoystickPOVDirection Value) override;
	virtual void JoystickBall(int32 DeviceId, int32 Ball, FVector2D Value) override;

	FJoystickDeviceData GetDeviceData(int32 DeviceId);
	FJoystickInfo GetDeviceInfo(int32 DeviceId);
	FDeviceSDL* GetDeviceSDL() const;
	int32 GetDeviceCount() const;
	void GetDeviceIds(TArray<int32>& DeviceIds) const;

private:
	void InitInputDevice(const FDeviceInfoSDL &Device);

	TMap<int32, FJoystickDeviceData> JoystickDeviceData;
	TMap<int32, FJoystickInfo> JoystickDeviceInfo;

	TMap<int32, TArray<FKey>> DeviceButtonKeys;
	TMap<int32, TArray<FKey>> DeviceAxisKeys;
	TMap<int32, TArray<FKey>> DeviceHatKeys[2];
	TMap<int32, TArray<FKey>> DeviceBallKeys[2];
	
	TSharedPtr<FDeviceSDL> DeviceSDL;
	
	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;
};
