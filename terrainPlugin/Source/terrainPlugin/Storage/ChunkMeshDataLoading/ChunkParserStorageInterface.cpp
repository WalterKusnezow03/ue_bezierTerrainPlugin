#include "ChunkParserStorageInterface.h"
#include "GameCore/MeshGenBase/materialHelper/MaterialEnumHelper.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"



ChunkParserStorageInterface::ChunkParserStorageInterface(){

}

ChunkParserStorageInterface::~ChunkParserStorageInterface(){

}

void ChunkParserStorageInterface::Save(
    FString worldLevelName, 
    ChunkParser &chunkData
){
    
    TArray<uint8> Bytes;


    //append mesh data
    TArray<MeshData *> meshDataArray = MeshDataOrderedForSavingAndLoading(chunkData);
    //append actor data
    WriteChunkInfoData(Bytes, chunkData, meshDataArray.Num());

   
    for (int i = 0; i < meshDataArray.Num(); i++){
        
        MeshData *currentPointer = meshDataArray[i];
        if(currentPointer){
            MeshData &meshDataCurrent = *currentPointer;
            //append meshdata from super method
            AppendIntoByteBuffer(
                Bytes, // buffer size is increased after append!
                meshDataCurrent.getVerteciesRef(),
                meshDataCurrent.getNormalsRef(),
                meshDataCurrent.getUV0Ref(),
                meshDataCurrent.getTrianglesRef()
            );
        }
    }

    // SAVE FILE Super::Super method.
    FString path = makePath(worldLevelName, chunkData.getChunkId());
    /*
    SaveBinaryData(
        FString Path,
        TArray<uint8> &Bytes
    )
    */
    if(SaveBinaryData(path, Bytes)){
        DebugHelper::logMessage("Storage Interface Chunk Parser Saved: ", path);
    }
}

//write at FRONT
void ChunkParserStorageInterface::WriteChunkInfoData(
    TArray<uint8> &bytes,
    ChunkParser &chunkData,
    int32 numLayers
){
    //Bin layout [actorLocation, waterLocation, waterFlag, outpostFlag]
    bytes.SetNumUninitialized(bytes.Num() + headerInfoDataSize()); //set new byte num (should not be needed, count 0)
    uint8 *Ptr = bytes.GetData();
    /*
    FMemory::Memcpy ( 
        void* Dest,
        const void* Src,
        SIZE_T Count  (in bytes if using uint8*)
    )
    */
    FVector actorLocation = chunkData.GetActorLocation();
    FMemory::Memcpy(Ptr, &actorLocation, sizeof(FVector));
    Ptr += sizeof(FVector);

    FVector waterActorLocation = chunkData.GetWaterActorLocation();
    FMemory::Memcpy(Ptr, &waterActorLocation, sizeof(FVector));
    Ptr += sizeof(FVector);


    bool waterActorNeeded = chunkData.WaterActorNeededFlagged();
    FMemory::Memcpy(Ptr, &waterActorNeeded, sizeof(bool));
    Ptr += sizeof(bool);
    
    bool flagOutpostNeeded = chunkData.OutpostFlagCreationNeeded();
    FMemory::Memcpy(Ptr, &flagOutpostNeeded, sizeof(bool));
    Ptr += sizeof(bool); 
    
    //num layers of mesh data
    FMemory::Memcpy(Ptr, &numLayers, sizeof(int32));
    //Ptr += sizeof(int); ////not needed, ptr is not used outside
}



bool ChunkParserStorageInterface::CanLoadChunkInfoData(TArray<uint8> &bytes){
    return headerInfoDataSize() <= bytes.Num();
}

//read at FRONT, increases the pointer to end!
void ChunkParserStorageInterface::LoadChunkInfoData(
    uint8* &Ptr,
    ChunkParser &chunkData,
    int32 &numLayersOut
){

    //Bin layout [actorLocation, waterLocation, waterFlag, outpostFlag]
    /*
    FMemory::Memcpy ( 
        void* Dest,
        const void* Src,
        SIZE_T Count  (in bytes if using unit8*)
    )
    */
    FVector actorLocation;
    FMemory::Memcpy(&actorLocation, Ptr, sizeof(FVector)); //copy casted data
    Ptr += sizeof(FVector);
    chunkData.SetActorLocation(actorLocation);


    FVector waterActorLocation;
    FMemory::Memcpy(&waterActorLocation, Ptr, sizeof(FVector)); //copy casted data
    Ptr += sizeof(FVector);

    bool waterFlag = false;
    FMemory::Memcpy(&waterFlag, Ptr, sizeof(bool)); //copy casted data
    Ptr += sizeof(bool);
    chunkData.SetWaterActorNeededFlag(waterFlag, waterActorLocation);
    
    
    bool outpostFlag = false;
    FMemory::Memcpy(&outpostFlag, Ptr, sizeof(bool)); //copy casted data
    Ptr += sizeof(bool);
    chunkData.SetOutpostFlagNeeded(outpostFlag);


    //num layers of mesh data
    FMemory::Memcpy(&numLayersOut, Ptr, sizeof(int32));
    Ptr += sizeof(int32); ////NEEDED ptr is not used outside
}

int ChunkParserStorageInterface::headerInfoDataSize(){
    return 
    sizeof(FVector) + //actor location
    sizeof(FVector) + //water location
    sizeof(bool) + //water flag
    sizeof(bool) + //outpost flag
    sizeof(int32); //count layers
}





bool ChunkParserStorageInterface::Load(
    FString worldLevelName, 
    ChunkParser &chunkData
){
    
    //for all lods
        //for all materials
            //load with mesh data interface

    /*
    //will load mesh data into buffers, EXPECTS INFO DATA AT POINTER, 
    //WILL INCREASE POINTER ALL THE WAY TO START OF NEXT BUFFER CHUNK / MESHDATA
    //MIGHT BE OUT OF BOUNCE BY THEN!
    void LoadIntoMeshBuffers(
        TArray<uint8> &Bytes, // buffer size is increased after append!
        uint8*& Ptr,           // is increased after append, must be at correct offset starting with header bytes!
        TArray<FVector> &Vertecies,
        TArray<FVector> &Normals,
        TArray<FVector2D> &UV0,
        TArray<int32> &Triangles,
        bool &endReached
    );
    */
    FString path = makePath(worldLevelName, chunkData.getChunkId());
    TArray<uint8> Bytes;
    if (!LoadBinaryData(path, Bytes))
    {
        FString message = FString::Printf(TEXT("Storage Interface chunk parser ERROR LOADING BIN DATA (%s)"), *path);
        DebugHelper::logMessage(message);
        return false;
    }else{
        FString message = FString::Printf(TEXT("Storage Interface chunk parser SUCCESS LOADING BIN DATA (%s)"), *path);
        DebugHelper::logMessage(message);
    }

    if(!CanLoadChunkInfoData(Bytes)){
        FString message = FString::Printf(
            TEXT("Storage Interface chunk parser ERROR LOADING BIN HEADER DATA (%s)"), *path
        );
        DebugHelper::logMessage(message);
        return false;
    }

    uint8 *Ptr = Bytes.GetData(); //global pointer for loading, because meshdata is concatenated

    //load actor data
    int numLayersFound = 0;
    LoadChunkInfoData(Ptr, chunkData, numLayersFound);

    //load mesh data
    TArray<MeshData *> meshDataArray = MeshDataOrderedForSavingAndLoading(chunkData);
    for (int i = 0; i < meshDataArray.Num(); i++){
        bool bEndReached = false;
        if(i < numLayersFound){
            MeshData *currentPointer = meshDataArray[i];
            if(currentPointer){
                MeshData &meshDataCurrent = *currentPointer;
                LoadIntoMeshBuffers(
                    Bytes, // buffer size is increased after append!
                    Ptr,   // is increased after append, must be at correct offset starting with header bytes!
                    meshDataCurrent.getVerteciesRef(),
                    meshDataCurrent.getNormalsRef(),
                    meshDataCurrent.getUV0Ref(),
                    meshDataCurrent.getTrianglesRef(),
                    bEndReached
                );
                meshDataCurrent.updateBoundsIfNeeded(); //very important here to update bounds from loaded data
            }
            if(bEndReached){
                break;
            }
        }else{
            break;
        }
    }

    return true;
}

FString ChunkParserStorageInterface::makePath(
    FString worldLevelName, 
    int chunkId
){
    FString fileName = FString::Printf(TEXT("Terrain/chunkMeshData/chunk_%d.bin"), chunkId);
    FString Path = BaseDir(worldLevelName) + fileName;
    return Path;
}





TArray<MeshData *> ChunkParserStorageInterface::MeshDataOrderedForSavingAndLoading(
    ChunkParser &chunkData
){
    TArray<MeshData *> outArray;

    //die schachtelungs reihenfolge von
    //(bool, lod, material) MUSS beim lesen genau so sein!
    //leere daten werden als leer eingespeichert.
    //Die lod, raycast flag und material reihen folge wird implizit gespeichert.
    //Ganz einfaches system.
    std::vector<bool> raycastFlags = {true, false};
    std::vector<ELod> lods = LodConstants::lodVector();
    std::vector<materialEnum> materials = MaterialEnumHelper::materialVector(); //issue here when no mesh data added ?
  
    for (int b = 0; b < raycastFlags.size(); b++){
        bool bRaycastFlag = raycastFlags[b];
        for (int i = 0; i < lods.size(); i++)
        {
            ELod lodCurrent = lods[i];
            for (int j = 0; j < materials.size(); j++)
            {
                materialEnum materialCurrent = materials[j];
                MeshData &ref = chunkData.findMeshDataReference(materialCurrent, lodCurrent, bRaycastFlag);
                outArray.Add(&ref);
            }
        }
    }

    return outArray;
}
