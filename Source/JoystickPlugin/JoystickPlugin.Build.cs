using System.IO;

namespace UnrealBuildTool.Rules
{
    public class JoystickPlugin : ModuleRules
    {
        public JoystickPlugin(ReadOnlyTargetRules Target) : base(Target)
        {
            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "ApplicationCore",
                    "InputCore",
                    "SlateCore",
                    "Slate"
                });

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "Projects",
                    "InputDevice",
                    "DeveloperSettings"
                });

            var SDL2IncPath = Path.Combine(EngineDirectory, "Source", "ThirdParty", "SDL2", "SDL-gui-backend", "include");

            PublicIncludePaths.Add(SDL2IncPath);

            var SDLDirectory = Path.Combine(PluginDirectory, "ThirdParty", "SDL2");
            var SDLPlatformDir = Path.Combine(SDLDirectory, Target.Platform.ToString());

            if (Target.Platform == UnrealTargetPlatform.Win64)
            {
                RuntimeDependencies.Add(Path.Combine(SDLPlatformDir, "SDL2.dll"));
                PublicAdditionalLibraries.Add(Path.Combine(SDLPlatformDir, "SDL2.lib"));

                PublicDelayLoadDLLs.Add("SDL2.dll");
            }
        }
    }
}