#pragma once


#include "AssetMakerEditorPlugin/AssetMaker/Task/MeshDataAssetTask.h"

#include "EditorMeshDataToStaticMeshTool.generated.h"

/// @brief can save MeshData as UStaticMesh in a directory of your liking. use in editor only.
/// do NOT remove macro, engine will crash otherwise :)
UCLASS()
class ASSETMAKEREDITORPLUGIN_API UEditorMeshDataToStaticMeshTool : public UObject{
    GENERATED_BODY()
public:

#if WITH_EDITOR

    /// @brief add before launch
    static void AppendTask(const MeshDataAssetTask &task);

    /// @brief executes all tasks, called by command setupped in AssetMakerEditorPlugin.h / .cpp
    static void Launch();
    

    UStaticMesh *CreateStaticMeshAsset(
        MeshDataAssetTask &task
    );

    static bool AssetAlreadyExists(FString path, FString nameAsset);

private:
    static void Log(FString msg);

    bool TaskCanBeExecuted(MeshDataAssetTask &task);

    FMeshDescription *GetLod(int32 lod, UStaticMesh *mesh);
    bool Build(TArray<const FMeshDescription *> lods, UStaticMesh *mesh);


    void Append(
        FMeshDescription *mesh,
        MeshDataAssetTask &task
    );

    void AppendAllVertecies(
        FMeshDescription &mesh,
        const TArray<FVector> &vertexBuffer,
        TArray<FVertexInstanceID> &outBuffer
    );

    bool TriangleIsValid(
        int32 t0, int32 t1, int32 t2,
        TArray<FVertexInstanceID> &vertexIdBuffer
    );

    FPolygonGroupID MakeMaterialLayer(FMeshDescription &mesh);

    void AppendAllTriangles(
        FMeshDescription &mesh,
        TArray<FVertexInstanceID> &vertexIdBuffer,
        const TArray<int32> &triangles,
        FPolygonGroupID &materialId
    );

    void AppendUVBuffer(
        FMeshDescription &mesh,
        MeshDataAssetTask &task,
        TArray<FVertexInstanceID> &vertexIdBuffer,
        int32 uvChannel
    );

    static TArray<MeshDataAssetTask> tasks;

#endif
};





