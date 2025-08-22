#pragma once 

#include "CoreMinimal.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/TerrainMeshDataParser/ChunkParser.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/ChunkSetup/TerrainChunkMap.h"
#include "terrainPlugin/Storage/ChunkMeshDataLoading/ChunkParserStorageInterface.h"

// will store generated or loaded from storage Chunks
class TERRAINPLUGIN_API ChunkParserMap {

public:
    ChunkParserMap();
    ~ChunkParserMap();

    // Should be set on start to ensure consistent naming
    // if not loaded, the world has no name. bad.
    void setWorldLevelName(FString name);

    // --- Storage Interface Integrated here: SaveInterface ---
    // A Chunkparser Map can save and load itself from storage
    // Changes to the terrain are not tracked yet!
    void saveWorldLevel();
    bool loadWorldLevel(FString name, TerrainChunkMap &loadedHeaderMap);

    //api for terrain generator aswell as internal helper
    //finds a chunkparser by index (i,j) to copy or read data from
    ChunkParser &findByIndex(int i, int j);

    //call on start / load level
    void createArray(int chunksPerAxis);

    bool mapIsEmpty();

    /// @brief do not modify or delete! Dont call any update methods on this object
    /// while using the object! Pointers may get invalid.
    TArray<ChunkParser *> allChunksForSetup();

    ///@brief finds a quad around a center of chunks which are in valid index space
    TArray<ChunkParser *> findQuadAreaFromCenter(FVector2D &center, int sizeAreaHalf);

    ///@brief finds a quad around a center of chunks which are in valid index space
    /// and are not in use by an custom mesh actor (flagged)
    TArray<ChunkParser *> findQuadAreaFromCenterWhichAreNotInUseFlagged(FVector2D &center, int sizeAreaHalf);

    

private:
    FString worldLevelName;
    int countAxisSaved = 0;

    TArray<TArray<ChunkParser>> innerChunkParserMap;

   

    ChunkParser fallback;
    ChunkParser &findById(int id);
    

    bool makeIndex(int id, int &i, int &j);
    int makeIdFromIndex(int i, int j);

    void clampIndex(int &i, int &j);
    bool indexIsValid(int i, int j);

    void saveChunk(int i, int j);
    void saveChunk(int id);
};