#include "ChunkMapStorageInterface.h"
#include "GameCore/DebugHelper.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/ChunkSetup/TerrainChunkAttributes.h"


int ChunkMapStorageInterface::bytesHeader(){
    return sizeof(int32) * 1; //single integer for array size
}

int ChunkMapStorageInterface::bytesData(int num){
    return num * sizeof(FTerrainChunkAttributes);
}



void ChunkMapStorageInterface::Save(
    FString worldName, 
    TerrainChunkMap &mapMinimal
){

    //create binary data from map

    //speicher layout als
    //num | data
    TArray<uint8> bytes;
    

    TArray<FTerrainChunkAttributes *> map = mapMinimal.getAll();
    makeBinary(map, bytes);

    SaveBinaryData(
        makePath(worldName),
        bytes
    );
}

int ChunkMapStorageInterface::countValid(TArray<FTerrainChunkAttributes *> &mapArray){
    int outNum = 0;
    for (int i = 0; i < mapArray.Num(); i++){
        FTerrainChunkAttributes *current = mapArray[i];
        if(current){
            outNum++;
        }
    }
    return outNum;
}

void ChunkMapStorageInterface::makeBinary(
    TArray<FTerrainChunkAttributes *> &mapArray,
    TArray<uint8> &bytes
){
    

    //make array of valid data
    int32 numChunks = 0;
    TArray<FTerrainChunkAttributes> validData;
    for (int i = 0; i < mapArray.Num(); i++){
        FTerrainChunkAttributes *current = mapArray[i];
        if(current){
            validData.Add(*current);
            numChunks++;
        }
    }

    int _bytesHeader = bytesHeader();
    int _bytesData = bytesData(numChunks);
    int totalData = _bytesHeader + _bytesData;

    bytes.SetNumUninitialized(totalData); //in byte, das ist richtig so.

    uint8 *Ptr = bytes.GetData();

    /*
    FMemory::Memcpy ( 
        void* Dest,
        const void* Src,
        SIZE_T Count  (in bytes if using unit8*)
    )
    */
    FMemory::Memcpy(Ptr, &numChunks, _bytesHeader); //copy casted data
    Ptr += _bytesHeader; //increase ptr for proper saving

    FMemory::Memcpy(Ptr, (uint8 *)validData.GetData(), _bytesData); //copy casted data


}




FString ChunkMapStorageInterface::makePath(FString worldName){
    FString fileName = FString::Printf(TEXT("Terrain/worldHeader/worldHeader_%s.bin"), *worldName);
    FString Path = BaseDir(worldName) + fileName;
    return Path;
}




bool ChunkMapStorageInterface::Load(
    FString worldName, 
    TerrainChunkMap &mapMinimal
){

    //load map from storage
    FString path = makePath(worldName);
    TArray<uint8> bytes;
    if(!LoadBinaryData(path, bytes)){
        DebugHelper::logMessage("Failed to load map header!");
        return false;
    }

    //find count from first integer
    uint8 *Ptr = bytes.GetData();
    int _bytesHeader = bytesHeader();
    int32 arrayCountExtracted = 0;

    /*
    FMemory::Memcpy ( 
        void* Dest, 
        const void* Src,
        SIZE_T Count  (in bytes if using unit8*)
    )
    */

    //WARUM DARF ICH INT32 NICHT AUF UINT8 CASTEN!!!
    FMemory::Memcpy(&arrayCountExtracted, Ptr, _bytesHeader); //copy casted data
    if(_bytesHeader <= 0 || arrayCountExtracted <= 0){
        DebugHelper::logMessage("Bytes header is empty");
        return false;
    }

    Ptr += _bytesHeader; //increase ptr for proper loading from offset 

    //convert binary to TArray<FTerrainChunkAttributes> 
    TArray<FTerrainChunkAttributes> dataArray;
    int _bytesData = bytesData(arrayCountExtracted);

    //block data size so copy is not invalid!
    //SetNumUninitialized to not call construct on every element!
    dataArray.SetNumUninitialized(arrayCountExtracted);

    //GetData muss als pointer gecatet werden damit
    //es zu 1 unit8 byte passt
    FMemory::Memcpy((uint8 *)dataArray.GetData(), Ptr, _bytesData); // copy casted data

    //override chunk map
    mapMinimal.constructQuadFrom(dataArray);
    return true;
}



// ---- TESTING ----
void ChunkMapStorageInterface::Test(){
    TerrainChunkMap mapMinimal;
    mapMinimal.createChunkMap(5);

    FString beforeSave = TEXT("StorageInterface Chunk Map") + mapMinimal.toString();
    DebugHelper::logMessage(beforeSave);

    FString worldName = TEXT("world_1_debug");
    Save(worldName, mapMinimal);

    TerrainChunkMap loadedMap;
    Load(worldName, loadedMap);
    FString afterLoad = TEXT("StorageInterface Chunk Map") + loadedMap.toString();
    DebugHelper::logMessage(afterLoad);
}
