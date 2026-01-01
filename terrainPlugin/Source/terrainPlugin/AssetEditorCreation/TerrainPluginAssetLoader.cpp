#include "TerrainPluginAssetLoader.h"



#include "AssetPlugin/gamestart/assetManager.h"
#include "AssetPlugin/gamestart/AssetLoader.h"
#include "terrainPlugin/AssetEnums/EFoliageGrass.h"


#include "AssetMakerEditorPlugin/AssetMaker/MeshDataToStaticMesh/EditorMeshDataToStaticMeshTool.h"
#include "AssetMakerEditorPlugin/AssetMaker/Task/MeshDataAssetTask.h"
#include "terrainPlugin/meshgen/foliage/grass/editorOnly/MatrixGrass.h"

#include "DebugPlugin/DebugHelper.h"

void TerrainPluginAssetLoader::InitOnLaunchModule(){
    InitGrassAssets();
}

void TerrainPluginAssetLoader::LoadGrassAssetOnBeginPlay(){
    bool found = false;
    if (assetManager *instance = assetManager::instance())
    {
        UStaticMesh *asset = instance->Find<EFoliageGrass, UStaticMesh>(EFoliageGrass::grassAssetDefault);
        if(asset){
            found = true;
        }
    }
    if(!found){
        AssetLoader::LoadAndSaveAssetToManager<EFoliageGrass, UStaticMesh>(
            EFoliageGrass::grassAssetDefault, // track in asset manager
            "terrainPlugin",                  // like "Game" for game or any other plugin name
            "Prefabs/foliage",                // like: "Prefabs/Weapons/attachments", no trailing slash
            "grassAssetDefault"               // Just the file name as displayed
        );
    }
}



void TerrainPluginAssetLoader::InitGrassAssets(){
    //if grass asset does not exist: create
    LoadGrassAssetOnBeginPlay();

    //load asset here (external)
	AssetLoader::LoadAndSaveAssetToManager<EFoliageGrass, UStaticMesh>(
		EFoliageGrass::grassAssetDefault, // track in asset manager
		"terrainPlugin",                  // like "Game" for game or any other plugin name
		"Prefabs/foliage",                // like: "Prefabs/Weapons/attachments", no trailing slash
		"grassAssetDefault"               // Just the file name as displayed
	);

    #if WITH_EDITOR
	if(assetManager *instance = assetManager::instance()){
		UStaticMesh *asset = instance->Find<EFoliageGrass, UStaticMesh>(EFoliageGrass::grassAssetDefault);
		if(!asset){
			//still not found create
            MatrixGrass instancer;
            MeshData generatedMeshData = instancer.Generate(100, 10);

            MeshDataAssetTask task;
            task.Update(
                generatedMeshData.getVerteciesRef(), 
                generatedMeshData.getTrianglesRef(),
                generatedMeshData.getUV0Ref()
            );
            task.UpdatePathAndName(TEXT("/terrainPlugin/Prefabs/foliage/"), TEXT("grassAssetDefault"));
            UEditorMeshDataToStaticMeshTool::AppendTask(task);
            //UEditorMeshDataToStaticMeshTool::Launch();
            //load again

            if(UStaticMesh *asset1 = instance->Find<EFoliageGrass, UStaticMesh>(EFoliageGrass::grassAssetDefault)){
                DebugHelper::logMessage("TerrainPluginAssetLoader created asset.");
            }
        }
	}
    #endif
}
