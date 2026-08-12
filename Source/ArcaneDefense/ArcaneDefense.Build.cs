// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ArcaneDefense : ModuleRules
{
	public ArcaneDefense(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"UMG",
			"AIModule",
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ArcaneDefense",
			"ArcaneDefense/Variant_Platforming",
			"ArcaneDefense/Variant_Platforming/Animation",
			"ArcaneDefense/Variant_Combat",
			"ArcaneDefense/Variant_Combat/AI",
			"ArcaneDefense/Variant_Combat/Animation",
			"ArcaneDefense/Variant_Combat/Gameplay",
			"ArcaneDefense/Variant_Combat/Interfaces",
			"ArcaneDefense/Variant_Combat/UI",
			"ArcaneDefense/Variant_SideScrolling",
			"ArcaneDefense/Variant_SideScrolling/AI",
			"ArcaneDefense/Variant_SideScrolling/Gameplay",
			"ArcaneDefense/Variant_SideScrolling/Interfaces",
			"ArcaneDefense/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
