#include "ActorManager.h"
#include "terrainPlugin/Storage/chunkMapHeaderLoading/ChunkMapStorageInterface.h"
#include "GameCore/EntityGC/EntityManagerBase.h"
#include "GameCore/PlayerInfo/PlayerInfo.h"
#include "GameCore/DebugHelper.h"
#include "GameCore/MeshGenBase/lodHelper/LodConstants.h"
#include "GameCore/world/worldLevelBase.h"

ActorManager::ActorManager(){
    worldContext = nullptr;
}

ActorManager::~ActorManager(){
    worldContext = nullptr;
}

void ActorManager::BeginPlay(FString worldLevelString, UWorld *world){
    findMaxChunkViewDistanceOnBeginPlay();
    worldContext = world;

    chunkMeshDataParserMap.setWorldLevelName(worldLevelString);

    //load header map (not all chunks may be generated all at once, header map needed)
    terrainHeaderFileWasFound = chunkHeaderMap.Load(worldLevelString);

    if(!terrainHeaderFileWasFound){
        chunkHeaderMap.createChunkMap(chunksForGame);
        generateTerrain();
    }
    else
    {

        //Option 1:
        //handled in tick to load proper chunk data / meshdata obj stuff.

        //Option 2: (for now)
        //load all mesh data into chunk parser map,
        //used by custom mesh actor later
        loadWorldMeshData(worldLevelString);
    }
}

// Override EndPlay
void ActorManager::EndPlay(){

    //save world header
    chunkHeaderMap.Save();

    //save world
    chunkMeshDataParserMap.saveWorldLevel();

    //mark all pointers as free, game doesnt need to end,
    //just terrain cleared.
    markAllActorsAsFree();
}

void ActorManager::loadWorldMeshData(FString worldLevelString){
    //try to load Chunk Data into chunkparser map.
    if(chunkMeshDataParserMap.loadWorldLevel(worldLevelString, chunkHeaderMap)){
        DebugHelper::logMessage("ActorManager: Chunks loaded from SSD");
        
        if(instantTerrainActorApply){
            applyChunkmeshDataCompletly();
            DebugHelper::logMessage("ActorManager: terrain loading from SSD finished!");
        }


    }else{
        DebugHelper::logMessage("ActorManager: terrain generation needed!");
        generateTerrain();
    }
}

//chunkMap must be already loaded / setup!
void ActorManager::generateTerrain(){
    //push data into terrain generator

    //setup chunk parser map
    chunkMeshDataParserMap.createArray(chunksForGame);
    terraincreator.createTerrainAndSetupChunkParserMap(chunkHeaderMap, chunkMeshDataParserMap);

    //debug wise gen all at once
    //later with updates.
    if(instantTerrainActorApply){
        applyChunkmeshDataCompletly();
        DebugHelper::logMessage("ActorManager: terrain generation finished!");
    }
}

void ActorManager::applyChunkmeshDataCompletly(){

    //run through chunk parser map
    //make custom mesh actor instance for each chunk parser
    //apply data
    //actor location is stored in chunk parser!

    //void AcustomMeshActor::UpdateMeshDataAndPosition(ChunkParser &parser)

    int madeActors = 0;
    TArray<ChunkParser *> setupChunks = chunkMeshDataParserMap.allChunksForSetup();
    for (int i = 0; i < setupChunks.Num(); i++){
        ChunkParser *currentParserPackage = setupChunks[i];
        if(currentParserPackage){

            //create terrain mesh actor
            AcustomMeshActor *actor = PopMeshActorFromFreeList();
            if (actor){
                actor->UpdateMeshDataAndPosition(*currentParserPackage);
                madeActors++;
            }

            //create outposts
            //single fire create outpost, once!
            if(currentParserPackage->OutpostFlagCreationNeeded()){
                FVector outpostLocation = currentParserPackage->GetActorLocation();
                AworldLevelBase::addOutpostAt(outpostLocation);
            }

            //create water
            if(currentParserPackage->WaterActorNeededFlagged()){
                FVector waterLocation = currentParserPackage->GetWaterActorLocation();
                AcustomWaterActor::createWaterPane(
                    worldContext,
                    waterLocation,
                    terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER
                );
            }

        }
    }
    DebugHelper::logMessage("ActorManager made actors", madeActors);
}



void ActorManager::findMaxChunkViewDistanceOnBeginPlay(){
    //constant meters
    int maxViewDistance = LodConstants::metersLodFarUntil * terrainConstants::ONEMETER;
    chunkViewDistance = maxViewDistance / terrainConstants::CHUNKSIZE;
}


AcustomMeshActor *ActorManager::PopMeshActorFromFreeList(){
    AcustomMeshActor *actor = nullptr;
    while(actor == nullptr){
        if(markedFreeForUse.Num() <= 0){
            break;
        }
        actor = markedFreeForUse.Pop();
    }
    //found a actor for use: return
    if(actor != nullptr){
        return actor;
    }

    //create terrain mesh actor
    actor = AcustomMeshActor::makeInstance(worldContext);
    return actor;
}

void ActorManager::markAllActorsAsFree(){
    for (int i = 0; i < inUse.Num(); i++){
        AcustomMeshActor *current = inUse[i];
        if(current){
            if(!markedFreeForUse.Contains(current)){
                markedFreeForUse.Add(current);
            }
        }
    }
    inUse.Empty();
}

void ActorManager::Tick(float deltatime){

    //listen for player location and load mesh data if needed
    FVector playerLocation = PlayerInfo::playerLocation();
    FVector2D playerLocationChunkSpace = inChunkSpace(playerLocation);

    //player location muss in chunks space umgewandelt werden
    //dann müssen alle chunk ids abgeholt werden die gebraucht werden
    
    //die information ob der chunk generiert wurde muss auch 
    //vorliegen, das muss im selben struct gespeichert werden, damit ich 
    //nicht von der festplatte lese ob da, wenn nicht, gen, speichern, laden. DOOF

    //es wird sofort ermittelt ob schonmal generiert,
    //und auch diese info muss gepsichert werden, der chunk map als minimale info
    //muss auch auf der festplatte gespeichert werden wie ein world header
    //terrain/worldHeader_1 (darin die map Binary!)

    //--> create derived storage interface class!


    //schonmal gespeichert bool true: load
    //else: generate and save, also add to some sort of 
    //request list as TArray<ChunkParser> request list -----> will provide mesh data map 
    //to ameshactor instance!

    //is not tested at all
    if(!instantTerrainActorApply){
        UpdateMeshActorLists();
        updateMeshActorsBasedOnPlayerLocation(playerLocationChunkSpace);
    }
    
}

FVector2D ActorManager::inChunkSpace(FVector &playerLocation){
    FVector2D output(
        terraincreator.cmToChunkIndex(playerLocation.X), 
        terraincreator.cmToChunkIndex(playerLocation.Y)
    );
    return output;
}



// could be refactured to notify pattern: AcustommeshActor could know Actor Manager!
// on switch to far lod: release.
void ActorManager::UpdateMeshActorLists(){

    //es muss die e lod abegefragt werden
    //dann in die free use list gepackt,


    // ---- terrain update ----
    //der chunk parser muss sich merken ob ein mesh data cache (er selbst) aktuell aktiv ist
    //nur wenn nicht, wird ein neuer mesh actor befüllt
    TArray<AcustomMeshActor *> updatedArrayInUse;
    for (int i = 0; i < inUse.Num(); i++)
    {
        AcustomMeshActor *current = inUse[i];
        if(current && current->GetCurrentLodLevel() == ELod::lodFar){
            if(!markedFreeForUse.Contains(current)){
                current->releaseChunkParserPointer(); //remove chunk parser pointer, free use chunk is marked in parser
                markedFreeForUse.Add(current);
            }
        }else{
            //add to updated list
            updatedArrayInUse.Add(current);
        }
    }
    inUse = updatedArrayInUse;
}



void ActorManager::updateMeshActorsBasedOnPlayerLocation(FVector2D &playerLocationChunkSpace){
    TArray<ChunkParser *> arrayToApplyToActor = 
    chunkMeshDataParserMap.findQuadAreaFromCenterWhichAreNotInUseFlagged(
        playerLocationChunkSpace,
        chunkViewDistance
    );
    for (int i = 0; i < arrayToApplyToActor.Num(); i++){
        //apply data
        ChunkParser *currentParserPackage = arrayToApplyToActor[i];
        if(currentParserPackage){
            AcustomMeshActor *actor = PopMeshActorFromFreeList();
            if(actor){
                actor->UpdateMeshDataAndPosition(*currentParserPackage);

                //move actors to used list
                inUse.Add(actor);
            }
        }
    }
}