// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AssetMakerEditorPlugin : ModuleRules
{
	public AssetMakerEditorPlugin(ReadOnlyTargetRules Target) : base(Target)
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
				"RenderCore",         // für Build/Renderdaten
				"RHI",                // falls du direkt VertexBuffer o.ä. anfassen willst
				"MeshDescription",    // für FMeshDescription, FMeshDescriptionBuilder
				"StaticMeshDescription", // für FStaticMeshAttributes
				"AssetRegistry",      // für AssetRegistryModule::AssetCreated
				"UnrealEd",           // nötig im Editor-Modul für Asset-Erstellung
				"EditorSubsystem",    // wenn du AssetEditorSubsystem benutzt
			}
		);
		
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
