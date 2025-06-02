// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DominionProtocol : ModuleRules
{
	public DominionProtocol(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"GameplayTags",
			"AIModule",
			"NavigationSystem",
			"GameplayTasks",
			"MissNoHit",
            "Niagara",
            "LevelSequence",
            "MovieScene",
            "MovieSceneTools",
            "MovieSceneTracks"
        });
		
		PrivateIncludePaths.Add("DominionProtocol");
	}
}
