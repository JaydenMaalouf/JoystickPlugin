// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogJoystickPlugin, Log, All);

class JOYSTICKPLUGIN_API FJoystickLogManager
{
public:
	static FJoystickLogManager* Get();

	template <class FmtType, class... Types>
	void Log(ELogVerbosity::Type Level, const FmtType& Fmt, Types... Args);

	template <class FmtType, class... Types>
	void LogWarning(const FmtType& Fmt, Types... Args);

	template <class FmtType, class... Types>
	void LogError(const FmtType& Fmt, Types... Args);

	template <class FmtType, class... Types>
	void LogDebug(const FmtType& Fmt, Types... Args);

	template <class FmtType, class... Types>
	void LogInformation(const FmtType& Fmt, Types... Args);

private:
	bool CanLog() const;
};
