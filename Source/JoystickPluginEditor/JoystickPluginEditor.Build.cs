using UnrealBuildTool;

public class JoystickPluginEditor : ModuleRules
{
	public JoystickPluginEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"EditorStyle",
				"SlateCore",
				"Slate",
				"JoystickPlugin",
				"PropertyEditor"
			});
	}
}