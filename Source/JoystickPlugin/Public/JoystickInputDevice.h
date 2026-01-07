// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "IInputDevice.h"
#include "InputCoreTypes.h"
#include "Data/JoystickDeviceState.h"
#include "Data/JoystickInstanceId.h"
#include "Data/ResultMessage.h"
#include "Data/Input/KeyPair.h"
#include "Data/Settings/JoystickInputDeviceConfiguration.h"
#include "Runtime/Launch/Resources/Version.h"

enum class EHatDirection : uint8;
enum class EInputDeviceConnectionState : uint8;
struct FDeviceInfoSDL;
struct FInputDeviceId;
struct FPlatformUserId;

class JOYSTICKPLUGIN_API FJoystickInputDevice final : public IInputDevice
{
public:
	explicit FJoystickInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);

	virtual void Tick(float DeltaTime) override;
	virtual void SendControllerEvents() override;
	virtual void SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	virtual void SetChannelValue(int ControllerId, FForceFeedbackChannelType ChannelType, float Value) override;
	virtual void SetChannelValues(int ControllerId, const FForceFeedbackValues& Values) override;
	virtual bool IsGamepadAttached() const override;

	void JoystickPluggedIn(const FDeviceInfoSDL& Device);
	void JoystickUnplugged(const FJoystickInstanceId& InstanceId, const FInputDeviceId& InputDeviceId) const;
	void JoystickButton(const FJoystickInstanceId& InstanceId, const int Button, const bool Pressed);
	void JoystickAxis(const FJoystickInstanceId& InstanceId, const int Axis, const float Value);
	void JoystickHat(const FJoystickInstanceId& InstanceId, const int Hat, const EHatDirection Value);
	void JoystickBall(const FJoystickInstanceId& InstanceId, const int Ball, const FVector2D& Value);
	void JoystickGyro(const FJoystickInstanceId& InstanceId, const FVector& Value);
	void JoystickAccelerometer(const FJoystickInstanceId& InstanceId, const FVector& Value);

	TTuple<FJoystickDeviceState*, FInternalResultMessage> GetDeviceState(const FJoystickInstanceId& InstanceId);
	void GetDeviceKeys(const FJoystickInstanceId& InstanceId, TArray<FKey>& Keys);
	const FKey& GetDeviceAxisKey(const FJoystickInstanceId& InstanceId, const int AxisIndex);
	const FKey& GetDeviceButtonKey(const FJoystickInstanceId& InstanceId, const int ButtonIndex);
	const FKeyPair& GetDeviceHatKey(const FJoystickInstanceId& InstanceId, int HatKeyIndex);
	FJoystickInstanceId GetInstanceIdByKey(const FKey& Key) const;
	int GetAxisIndexFromKey(const FKey& Key) const;
	int GetButtonIndexFromKey(const FKey& Key) const;

	void ResetAxisProperties();
	void ResetButtonProperties();
	void UpdateDeviceProperties();
	void UpdateAxisProperties(FJoystickInputDeviceConfiguration* DeviceConfig, FJoystickDeviceState& CurrentState) const;
	void UpdateButtonProperties(FJoystickInputDeviceConfiguration* DeviceConfig, FJoystickDeviceState& CurrentState) const;

private:
	void InitialiseAxis(const FJoystickInstanceId& InstanceId, FJoystickInputDeviceConfiguration* DeviceConfiguration, const FString& BaseKeyName, const FString& BaseDisplayName);
	void InitialiseButtons(const FJoystickInstanceId& InstanceId, FJoystickInputDeviceConfiguration* DeviceConfiguration, const FString& BaseKeyName, const FString& BaseDisplayName);
	void InitialiseHatAxis(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName, const bool PairedKey);
	void InitialiseHatButtons(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName);
	void InitialiseBalls(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName, const bool PairedKey);
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
	void NotifyDeviceState(const FInputDeviceId& InputDeviceId, const FPlatformUserId& PlatformUserId, const EInputDeviceConnectionState State) const;
#endif
	void TryAddWidgetNavigation(const FKey& ButtonKey) const;

	TMap<FJoystickInstanceId, FJoystickDeviceState> JoystickDeviceState;

	TMap<FJoystickInstanceId, TArray<FKey>> DeviceButtonKeys;
	TMap<FJoystickInstanceId, TArray<FKey>> DeviceAxisKeys;
	TMap<FJoystickInstanceId, TArray<FKeyPair>> DeviceHatAxisKeys;
	TMap<FJoystickInstanceId, TArray<FKey>> DeviceHatAxisPairedKeys;
	TMap<FJoystickInstanceId, TArray<FKey>> DeviceHatButtonKeys;
	TMap<FJoystickInstanceId, TArray<FKeyPair>> DeviceBallKeys;
	TMap<FJoystickInstanceId, TArray<FKey>> DeviceBallPairedKeys;
	TMap<FJoystickInstanceId, TArray<FKey>> DeviceKeys;

	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;
};
