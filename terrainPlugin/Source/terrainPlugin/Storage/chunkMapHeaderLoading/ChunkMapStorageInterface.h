#pragma once

#include "CoreMinimal.h"
#include "StoragePlugin/Storage/BaseInterface/StorageInterface.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/ChunkSetup/TerrainChunkMap.h"


class TERRAINPLUGIN_API ChunkMapStorageInterface : public StorageInterface {

public:
    ChunkMapStorageInterface(){};
    virtual ~ChunkMapStorageInterface() override {};

    /// @brief saves a map header file
    /// @param worldName 
    /// @param mapMinimal 
    void Save(FString worldName, TerrainChunkMap &mapMinimal);

    /// @brief loads a map header file by world name, if valid, true is returned
    /// @param worldName 
    /// @param outMap 
    /// @return 
    bool Load(FString worldName, TerrainChunkMap &outMap);

    void Test();

private:
    int bytesHeader();
    int bytesData(int num);
    int countValid(TArray<FTerrainChunkAttributes *> &mapArray);
    void makeBinary(
        TArray<FTerrainChunkAttributes *> &mapArray,
        TArray<uint8> &bytes
    );

    FString makePath(FString worldName);
};