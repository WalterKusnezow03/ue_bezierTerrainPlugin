#pragma once

#include "CoreMinimal.h"
#include "TerrainChunkAttributes.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/terrainHillSetup.h"

// CHUNK PREDEFINITION - HEADER FOR TERRAIN
//will setup the chunk predefinition with heights
//minimal memory usuage by saving chunks and their height
// will always save a QUAD of chunks!
class TERRAINPLUGIN_API TerrainChunkMap {

public:
    TerrainChunkMap();
    ~TerrainChunkMap();

    /// @brief creates and generates a new chunk map 
    /// @param chunksIn 
    void createChunkMap(int chunksIn);

    /// @brief gets a quad within the given bounds from start(x,y) + (chunks, chunks)
    TArray<FTerrainChunkAttributes *> getQuad(int x, int y, int chunksToCopy);

    /// @brief gets all chunks
    TArray<FTerrainChunkAttributes *> getAll();

    /// @brief total chunks of map
    int countTotalCunks();

    /// @brief count per axis (10 for example)->(totalcount = 10 * 10 = 100) 
    int countPerAxis();

    /// @brief constructs a quad from the data, sqrt(data.Num()) should be a integer
    void constructQuadFrom(TArray<FTerrainChunkAttributes> &data);

    /// @brief creates string information 
    FString toString();

    // --- Save and Load Interface ---
    void Save();
    bool Load(FString worldName);

private:
    FString worldLevelNameSaved;

    void override(int i, int j, FTerrainChunkAttributes &attributes);

    void verfiyChunks(int chunksIn);
    void createArray();
    void createRandomHeightMap();
    void createRandomHeightMapChunkWide(int layers);

    terrainHillSetup createRandomHillData();
    terrainHillSetup createRandomHillDataLargeScale();
    terrainHillSetup createRandomHillData(
        int sizeX,
        int sizeY,
        int heightMin
    );
    void applyHillData(std::vector<terrainHillSetup> &hillDataVec);
    void applyHillData(terrainHillSetup &hillData);

    FTerrainChunkAttributes &find(int i, int j);
    void override(FTerrainChunkAttributes &attributes, int i, int j);

    void verifyIndex(int &index);
    int clampIndex(int index);
    bool indexIsValid(int i, int j);

    /// @brief chunks per single Quad axis. 10 chunks, 10*10 map = 100!
    int chunks = 10;
    TArray<TArray<FTerrainChunkAttributes>> innerMap;


    //inset for hil data!
    int insetHillData = 1;
    int clampIndexInset(int index);



    //call AFTER Hill generation!
    void createRandomOutpostFlags();
    void createRandomOutpostFlagAndSmoothArea();
};