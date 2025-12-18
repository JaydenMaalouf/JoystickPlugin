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
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"UnrealEd",
			"ToolMenus",
			"LevelEditor",
			"PropertyEditor",
		});
	}
}