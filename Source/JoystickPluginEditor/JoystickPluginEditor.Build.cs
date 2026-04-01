// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

using UnrealBuildTool;

public class JoystickPluginEditor : ModuleRules
{
	public JoystickPluginEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"InputCore",
			"JoystickPlugin", 
			"SettingsEditor",
			"PropertyEditor"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"UnrealEd",
			"Kismet",
			"KismetWidgets",
			"ToolMenus",
			"LevelEditor",
			"EditorStyle",
			"PropertyEditor"
		});
	}
}