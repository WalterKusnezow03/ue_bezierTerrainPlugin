// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CustomMaterialPlugin : ModuleRules
{
	public CustomMaterialPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);


		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine"
				// ... add other public dependencies that you statically link with here ...
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	

				//using editor for materials:
				//"UnrealEd",
				//"MaterialEditor",
				//"MaterialUtilities"
			}
		);



		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);


		// Editor-only modules
		if (Target.bBuildEditor)
		{

			PrivateDependencyModuleNames.AddRange(new string[]
			{
				"UnrealEd",
				"MaterialEditor",
				"MaterialUtilities"
			});
		}

		//vertsteht ich nicht, für cook, package die abfrage
		if (Target.Type == TargetType.Editor)
		{
			PrivateDependencyModuleNames.AddRange(new string[]
			{
				"UnrealEd",
				"MaterialEditor",
				"MaterialUtilities"
			});
		}
	}
}
