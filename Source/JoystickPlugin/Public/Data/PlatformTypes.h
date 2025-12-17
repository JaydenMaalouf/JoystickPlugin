// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Misc/CoreMiscDefines.h"

#if ENGINE_MAJOR_VERSION == 4 || ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 0
struct FInputDeviceId
{
	static FInputDeviceId CreateFromInternalId(int32 InInternalId)
	{
		FInputDeviceId IdToReturn;
		IdToReturn.InternalId = InInternalId;
		return IdToReturn;
	}
	
	int32 GetId() const
	{
		return InternalId;
	}

	bool IsValid() const
	{
		return InternalId >= 0;
	}
	
private:
	int32 InternalId = INDEX_NONE;
};
#endif
