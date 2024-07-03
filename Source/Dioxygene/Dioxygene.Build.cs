// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Dioxygene : ModuleRules
{
	public Dioxygene(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
		//Steamworks
		PublicDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemNull", "OnlineSubsystemSteam"});
		PublicDependencyModuleNames.AddRange(new string[] { "Steamworks", "OnlineSubsystemUtils"});
	}
}
