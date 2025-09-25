// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetMakerEditorPlugin.h"
#include "AssetMakerEditorPlugin/AssetMaker/MeshDataToStaticMesh/EditorMeshDataToStaticMeshTool.h"

#define LOCTEXT_NAMESPACE "FAssetMakerEditorPluginModule"


void FAssetMakerEditorPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	static FAutoConsoleCommand Cmd(
		TEXT("AssetMakerEditorPluginModule.StaticLaunch"),
		TEXT("Test Static Launch"),
		FConsoleCommandDelegate::CreateStatic(&FAssetMakerEditorPluginModule::CommandLaunch)
	);

}

void FAssetMakerEditorPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAssetMakerEditorPluginModule, AssetMakerEditorPlugin)