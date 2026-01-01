#include "ChunkParserMap.h"
#include "DebugPlugin/DebugHelper.h"

ChunkParserMap::ChunkParserMap(){
    
}

ChunkParserMap::~ChunkParserMap(){

}

void ChunkParserMap::setWorldLevelName(FString name){
    worldLevelName = name;
}



void ChunkParserMap::createArray(int chunksPerAxis){
    chunksPerAxis = std::abs(chunksPerAxis);
    countAxisSaved = chunksPerAxis;

    innerChunkParserMap.SetNum(countAxisSaved);
    int id = 0;
    for (int i = 0; i < innerChunkParserMap.Num(); i++)
    {
        innerChunkParserMap[i] = TArray<ChunkParser>();
        TArray<ChunkParser> &ref = innerChunkParserMap[i];
        ref.SetNum(countAxisSaved);
        for (int j = 0; j < ref.Num(); j++){
            ChunkParser &current = ref[j];
            current.setChunkId(id); //for loading easier interface
            id++;
        }
    }
}



/// ----- SAVE AND LOAD INTERFACE -----

void ChunkParserMap::saveWorldLevel(){
    //TODO!
    ChunkParserStorageInterface interface;
    TArray<ChunkParser *> allChunksToSave = allChunksForSetup(); //to save
    for (int i = 0; i < allChunksToSave.Num(); i++){
        //is id already set? Needed inside Storage Interface!
        ChunkParser *current = allChunksToSave[i];
        if(current){
            interface.Save(worldLevelName, *current);
        }
    }
}



bool ChunkParserMap::loadWorldLevel(FString name, TerrainChunkMap &loadedHeaderMap){
    setWorldLevelName(name);
    int countChunksToLoad = loadedHeaderMap.countTotalCunks();
    int sizeMapAxis = loadedHeaderMap.countPerAxis();

    DebugHelper::logMessage(FString::Printf(TEXT("Storage Interface Chunk Parser Map Try loading Chunks %d "), countChunksToLoad));
    createArray(sizeMapAxis); // inner map needs to be set up!

    //for all chunks
    int loadedSuccesfully = 0;
    ChunkParserStorageInterface interface;

    TArray<ChunkParser *> chunksToLoad = allChunksForSetup();
    for (int i = 0; i < chunksToLoad.Num(); i++){
        ChunkParser *currentChunkParser = chunksToLoad[i];
        if(currentChunkParser){
            if(interface.Load(worldLevelName, *currentChunkParser)){
                loadedSuccesfully++;
            }
        }
    }


    FString message = FString::Printf(TEXT("Storage Interface Loaded World: (%d / %d)"), loadedSuccesfully, countChunksToLoad);
    DebugHelper::logMessage(message);

    /*
    ACHTUNG: TODO:

    Achtung: Wenn chunks asynchron generiert werden und nicht zwingend alle direkt generiert werden:
    So gilt dass die (loadedSuccesfully == countChunksToLoad) nur gilt,
    wenn alle generiert wurden und alle geladen werden sollen!
    */


    return loadedSuccesfully == countChunksToLoad;
}




ChunkParser &ChunkParserMap::findById(int id){
    int i = 0;
    int j = 0;
    if(makeIndex(id, i, j)){
        return findByIndex(i, j);
    }
    return fallback;
}

ChunkParser &ChunkParserMap::findByIndex(int i, int j){
    if(i >= 0 && i < innerChunkParserMap.Num()){
        if(j >= 0 && j < innerChunkParserMap[i].Num()){
            return innerChunkParserMap[i][j];
        }
    }
    return fallback;
}

bool ChunkParserMap::makeIndex(int id, int &i, int &j){
    int squareSize = countAxisSaved * countAxisSaved;
    if(id >= 0 && id < squareSize && countAxisSaved > 0){
        i = id % countAxisSaved; //wenn 19, 9 array trotzdem
        j = id / countAxisSaved;
        return true;
    }
    return false;
}

int ChunkParserMap::makeIdFromIndex(int i, int j){
    return j * countAxisSaved + i;
}

void ChunkParserMap::clampIndex(int &i, int &j){
    i = std::max(0, i);
    j = std::max(0, j);
    i = std::min(i, innerChunkParserMap.Num() - 1);
    if(i >= 0 && i < innerChunkParserMap.Num()){
        TArray<ChunkParser> &innerArray = innerChunkParserMap[i];
        j = std::min(j, innerArray.Num() - 1);
    }else{
        j = 0; //invalid at all
    }
}

bool ChunkParserMap::indexIsValid(int i, int j){
    bool iValid = i >= 0 && i < innerChunkParserMap.Num();
    bool jValid = false;
    if(iValid){
        jValid = j >= 0 && j < innerChunkParserMap[i].Num();
    }
    return iValid && jValid;
}

bool ChunkParserMap::mapIsEmpty(){
    return innerChunkParserMap.Num() <= 0;
}

void ChunkParserMap::saveChunk(int i, int j){
    //TArray<TArray<ChunkParser>> chunkParserMap;
    if(i >= 0 && i < innerChunkParserMap.Num()){
        if(j >= 0 && j < innerChunkParserMap[i].Num()){

        }
    }

}

void ChunkParserMap::saveChunk(int id){
    //TArray<TArray<ChunkParser>> chunkParserMap;
}




// Api for data copy!
TArray<ChunkParser *> ChunkParserMap::allChunksForSetup(){
    TArray<ChunkParser *> outArray;
    for (int i = 0; i < innerChunkParserMap.Num(); i++){
        for (int j = 0; j < innerChunkParserMap[i].Num(); j++){
            ChunkParser &ref = innerChunkParserMap[i][j];
            outArray.Add(&ref);
        }
    }
    return outArray;
}


///@brief finds a quad around a center of chunks which are in valid index space
TArray<ChunkParser *> ChunkParserMap::findQuadAreaFromCenter(
    FVector2D &center, 
    int sizeAreaHalf
){
    TArray<ChunkParser *> outArray;
    if(mapIsEmpty()){
        return outArray;
    }

    sizeAreaHalf = std::abs(sizeAreaHalf);
    int iStart = center.X - sizeAreaHalf;
    int jStart = center.Y - sizeAreaHalf;
    int iEnd = center.X + sizeAreaHalf + 1; //inclusive index
    int jEnd = center.Y + sizeAreaHalf + 1;

    clampIndex(iStart, jStart);
    clampIndex(iEnd, jEnd);

    for (int i = iStart; i < iEnd; i++){
        for (int j = jStart; j < jEnd; j++){
            if(indexIsValid(i,j)){
                outArray.Add(&findByIndex(i, j));
            }
        }
    }
    return outArray;
}

TArray<ChunkParser *> ChunkParserMap::findQuadAreaFromCenterWhichAreNotInUseFlagged(
    FVector2D &center, 
    int sizeAreaHalf
){
    TArray<ChunkParser *> outArrayUnfiltered = findQuadAreaFromCenter(center, sizeAreaHalf);
    TArray<ChunkParser *> outArrayValidated;
    for (int i = 0; i < outArrayUnfiltered.Num(); i++){
        ChunkParser *current = outArrayUnfiltered[i];
        if(current && !current->IsUsedByActor()){
            outArrayValidated.Add(current);
        }
    }
    return outArrayValidated;
}
