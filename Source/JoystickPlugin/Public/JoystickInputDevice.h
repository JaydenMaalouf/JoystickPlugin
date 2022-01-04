#pragma once

#include "IInputDevice.h"
#include "InputCoreTypes.h"
#include "Containers/Array.h"

#include "Data/DeviceInfoSDL.h"
#include "Data/JoystickDeviceData.h"
#include "Data/JoystickInfo.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"

class FJoystickInputDevice final : public IInputDevice
{
	
public:

	FJoystickInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);
		
	virtual void Tick(float DeltaTime) override;
	virtual void SendControllerEvents() override;
	virtual void SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override;
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues& Values) override;

	void ReinitialiseJoystickData(int32 DeviceId, FJoystickDeviceData InitialState);
	
	void JoystickPluggedIn(const FDeviceInfoSDL &Device);
	void JoystickUnplugged(int32 DeviceId);
	void JoystickButton(int32 DeviceId, int32 Button, bool Pressed);
	void JoystickAxis(int32 DeviceId, int32 Axis, float Value);
	void JoystickHat(int32 DeviceId, int32 Hat, EJoystickPOVDirection Value);
	void JoystickBall(int32 DeviceId, int32 Ball, FVector2D Value);

	FJoystickDeviceData GetDeviceData(int32 DeviceId);
	FJoystickInfo GetDeviceInfo(int32 DeviceId);
	int32 GetDeviceCount() const;
	void GetDeviceIds(TArray<int32>& DeviceIds) const;

	void SetPlayerOwnership(int32 DeviceId, int32 PlayerId);
	
private:
	void InitInputDevice(const FDeviceInfoSDL &Device);
	
	TMap<int32, FJoystickDeviceData> JoystickDeviceData;
	TMap<int32, FJoystickInfo> JoystickDeviceInfo;

	TMap<int32, TArray<FKey>> DeviceButtonKeys;
	TMap<int32, TArray<FKey>> DeviceAxisKeys;
	TMap<int32, TArray<FKey>> DeviceHatKeys[2];
	TMap<int32, TArray<FKey>> DeviceBallKeys[2];
	
	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;
};
