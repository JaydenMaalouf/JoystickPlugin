// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Data/JoystickInformation.h"

FJoystickInformation::FJoystickInformation() : Type(EJoystickType::Unknown)
                                               , ProductId(0)
                                               , FirmwareVersion(0)
                                               , ProductVersion(0)
                                               , VendorId(0)
                                               , IsGamepad(false)
                                               , Connection(EJoystickConnectionType::Unknown)
                                               , CollectionIndex(INDEX_NONE)
{
}

bool FJoystickInformation::IsCollectionSiblingOf(const FJoystickInformation& Other) const
{
	return InstanceId != Other.InstanceId &&
		Other.CollectionIndex != INDEX_NONE &&
		CollectionGroup.IsEmpty() == false &&
		CollectionGroup == Other.CollectionGroup &&
		ProductGuid == Other.ProductGuid;
}

void FJoystickInformation::UpdateCollectionFromPath()
{
	CollectionIndex = INDEX_NONE;
	CollectionGroup.Reset();

#if PLATFORM_WINDOWS
	const int32 GuidMarker = Path.Find(TEXT("#{"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	if (GuidMarker == INDEX_NONE)
	{
		return;
	}

	const int32 LastAmpersand = Path.Left(GuidMarker).Find(TEXT("&"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
	if (LastAmpersand == INDEX_NONE)
	{
		return;
	}

	const int32 InstanceMarker = Path.Left(LastAmpersand).Find(TEXT("#"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
	if (InstanceMarker == INDEX_NONE)
	{
		return;
	}

	const FString IndexString = Path.Mid(LastAmpersand + 1, GuidMarker - LastAmpersand - 1);
	if (!IndexString.IsNumeric())
	{
		return;
	}

	CollectionIndex = FCString::Atoi(*IndexString);
	CollectionGroup = Path.Mid(InstanceMarker + 1, LastAmpersand - InstanceMarker - 1);
#else
	//TODO: Other platform support
#endif
}

void FJoystickInformation::UpdateDeviceHash()
{
	const FString FullSignature = FString::Printf(TEXT("%s::%04X:%04X"),
	                                              *DeviceName.TrimStartAndEnd(),
	                                              VendorId,
	                                              ProductId
	);

	DeviceHash = FString::Printf(TEXT("%08X"), GetTypeHash(FullSignature));
}

FString FJoystickInformation::GetDeviceDisplayName() const
{
	return !DeviceName.IsEmpty() ? DeviceName : SafeDeviceName;
}
