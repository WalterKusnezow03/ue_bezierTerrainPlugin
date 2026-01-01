#pragma once

#include "CoreMinimal.h"


#include "GameCore/util/ActorBase/ActorBase.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"

/// @brief extracts meshdata from staticmesh render data
class GAMECORE_API MeshExtractor {
public:
    MeshExtractor();
    ~MeshExtractor();

    void ExtractMeshDataPerSection(AActorBase *actor, TArray<MeshData> &data);

    void ExtractMeshDataPerSection(
        TArray<UStaticMeshComponent *> &components,
        TArray<MeshData> &meshdata
    );
    void ExtractMeshDataPerSection(UStaticMeshComponent *comp, TArray<MeshData> &meshdata);
    void ExtractMeshDataPerSection(UStaticMesh *comp, TArray<MeshData> &meshdata);
    void ExtractMeshDataPerSection(
        const FStaticMeshRenderData *renderdata,
        TArray<MeshData> &meshdataArray,
        int32 LODIndex
    );

    //single meshdata copy
    void ExtractSingleMeshData(AActorBase *actor, MeshData &meshdata);
    void ExtractSingleMeshData(
        TArray<UStaticMeshComponent *> &components,
        MeshData &meshdata
    );

    void ExtractSingleMeshData(UStaticMeshComponent *comp, MeshData &meshData);
    void ExtractSingleMeshData(
        UStaticMesh *staticmesh,
        MeshData &singleMeshData,
        int32 LODIndex
    );

    void ExtractSingleMeshData(
        const FStaticMeshRenderData *renderdata,
        MeshData &singleMeshData,
        int32 LODIndex
    );

private:
    FVector ToVectorDouble(FVector3f vertex);
};