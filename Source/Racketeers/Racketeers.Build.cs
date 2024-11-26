// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Racketeers : ModuleRules
{
	public Racketeers(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OnlineSubsystem", "OnlineSubsystemUtils", "EnhancedInput" , "UMG", "Slate", "SlateCore", "HeadMountedDisplay" });
	}
}
