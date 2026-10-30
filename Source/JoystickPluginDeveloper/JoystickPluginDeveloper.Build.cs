// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

using UnrealBuildTool;

public class JoystickPluginDeveloper : ModuleRules
{
	public JoystickPluginDeveloper(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"JoystickPlugin"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"InputCore",
			"SlateCore"
		});
	}
}
