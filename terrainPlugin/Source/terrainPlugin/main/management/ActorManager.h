#pragma once
#include "CoreMinimal.h"
#include "terrainPlugin/meshgen/customMeshActor.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/ChunkSetup/TerrainChunkMap.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/terrainCreator.h"

/// @brief will listen for player location and re use actors, load meshes from storage
/// and apply to existing mesh actors
class TERRAINPLUGIN_API ActorManager {

public:
    ActorManager();
    ~ActorManager();

    /// @brief starts a world or tries to find from storage
    void BeginPlay(FString worldLevelString, UWorld *world);
    
    /// @brief end play - must be called from terrain launcher!
    void EndPlay();

    void Tick(float deltatime);

private:
    void markAllActorsAsFree();

    int chunksForGame = 10; //fixed size for this world, ONE AXIS
    bool instantTerrainActorApply = true;
    bool terrainHeaderFileWasFound = false;

    //finds and sets the "chunkViewDistance" on begin play for later dynamic mesh actor movement
    void findMaxChunkViewDistanceOnBeginPlay();
    int chunkViewDistance = 4;

    int actorCount = 100; //max count (10x10 chunks)
    
    void loadWorldMeshData(FString worldLevelString);

    /// @brief generates the complete terrain and pushes dtaa into chunkparser map.
    void generateTerrain();

    void UpdateMeshActorLists();
    FVector2D inChunkSpace(FVector &playerLocation);

    //debug method, generates all mesh actors immidiatly from chunk parser map
    void applyChunkmeshDataCompletly();

    //apply chunks based on player location and view distance
    void updateMeshActorsBasedOnPlayerLocation(FVector2D &playerLocationChunkSpace);

    //pops free actor from list or creates a new one.
    AcustomMeshActor *PopMeshActorFromFreeList();
    TArray<AcustomMeshActor *> inUse;
    TArray<AcustomMeshActor *> markedFreeForUse;

    UWorld *worldContext = nullptr;

    //generates height data minimal chunks at start if nothing found
    TerrainChunkMap chunkHeaderMap;

    //saves Chunks mesh data / loads them if possible
    ChunkParserMap chunkMeshDataParserMap;

    //use for generting meshes
    terrainCreator terraincreator;

    
};