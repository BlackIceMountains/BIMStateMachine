// Copyright Black Ice Mountains. All Rights Reserved.

using UnrealBuildTool;

public class BIMStateMachine: ModuleRules
{
    public BIMStateMachine(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
        });
    }
}
