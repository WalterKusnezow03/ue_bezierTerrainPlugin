// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "AssetMakerEditorPlugin/AssetMaker/MeshDataToStaticMesh/EditorMeshDataToStaticMeshTool.h"

class FAssetMakerEditorPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

    static void CommandLaunch()
    {
		#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("FAssetMakerEditorPluginModule Launched!"));
		UEditorMeshDataToStaticMeshTool::Launch();
		#endif
	}
};
