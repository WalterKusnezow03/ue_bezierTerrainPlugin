#pragma once

#include "CoreMinimal.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/TerrainMeshDataParser/ChunkParser.h"
#include "StoragePlugin/Storage/VertexData/StorageInterface/StorageInterfaceMeshData.h"
#include "AssetEnumCollection/assetEnums/materialEnum.h"
#include "terrainPlugin/Storage/ChunkMeshDataLoading/MeshDataIdentifier.h"


/// @brief will load and save ChunkParser files for custom mesh actor, by passing
/// references which are filled or read from
class TERRAINPLUGIN_API ChunkParserStorageInterface : public StorageInterfaceMeshData {

public:
    ChunkParserStorageInterface();
    ~ChunkParserStorageInterface();

    //save single chunk. Will load all mesh data and materials, lods automatically
    void Save(FString worldLevelname, ChunkParser &chunkData);
    bool Load(FString worldLevelname, ChunkParser &chunkData);

private:
    FString makePath(FString worldLevelName, int chunkId);

    /// gets all mesh data pointers from chunkData for loading and saving. 
    /// keeps the order consistent so no errors can happen. Empty MeshData are saved empty. 
    /// the mesh data lod and material are saved implicitly by order of this meshdata array.
    /// if the order doesnt change. Nothing changes and it keeps working.
    /*TArray<MeshData *> MeshDataOrderedForSavingAndLoading(
        ChunkParser &chunkData
    );*/
    
    
    TArray<FMeshDataIdentifier> MeshDataOrderedForSavingAndLoading(
        ChunkParser &chunkData
    );

    ///bytes size of header data (actor locations, water locations ...)
    int headerInfoDataSize();

    //will write the chunk info data into the byte array
    void WriteChunkInfoData(
        TArray<uint8> &bytes,
        ChunkParser &chunkData,
        int32 numLayers
    );

    //will load the chunk info data into the chunkdata object, from the given pointer(should be at 0)
    //pointer will be increased for later use!
    void LoadChunkInfoData(
        uint8 *&Ptr, 
        ChunkParser &chunkData,
        int32 &numLayersOut
    );
    bool CanLoadChunkInfoData(TArray<uint8> &bytes);
};