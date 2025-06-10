// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TopDownGame_1 : ModuleRules
{
	public TopDownGame_1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput","UMG" });
        PrivateDependencyModuleNames.AddRange(new string[] {"Slate","SlateCore", "MagicNodeRuntime" });
        PublicIncludePaths.AddRange(new string[] { "TopDownGame_1/Public/Utility" });
        PrivateIncludePaths.AddRange(new string[] { "TopDownGame_1/Private/Utility" });
    }
}
