#pragma once

#include "DeviceInfoHID.generated.h"

USTRUCT(BlueprintType)
struct FDeviceInfoHID
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString DevicePath;

	UPROPERTY(BlueprintReadOnly)
	int32 VendorId;

	UPROPERTY(BlueprintReadOnly)
	int32 ProductId;

	UPROPERTY(BlueprintReadOnly)
	FString SerialNumber;

	UPROPERTY(BlueprintReadOnly)
	int32 ReleaseNumber;

	UPROPERTY(BlueprintReadOnly)
	FString ManufacturerString;

	UPROPERTY(BlueprintReadOnly)
	FString ProductString;

	UPROPERTY(BlueprintReadOnly)
	int32 UsagePage;

	UPROPERTY(BlueprintReadOnly)
	int32 Usage;

	UPROPERTY(BlueprintReadOnly)
	int InterfaceNumber;

	UPROPERTY(BlueprintReadOnly)
	int InterfaceClass;

	UPROPERTY(BlueprintReadOnly)
	int InterfaceSubclass;

	UPROPERTY(BlueprintReadOnly)
	int InterfaceProtocol;
};
