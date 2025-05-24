

#include "TerrainChunkSetup.h"
#include "GameCore/DebugHelper.h"
#include "GameCore/world/worldLevelBase.h"


TerrainChunkSetup::TerrainChunkSetup(
    std::vector<std::vector<FVector>> &refIn,
    ETerrainType typeIn,
    bool createOutpostIn,
    FVector &outpostLocationIn,
    bool blockTreesIn
){
    setMapReference(refIn);
    savedTerrainType = typeIn;
    createOutpost = createOutpostIn;
    blockTrees = blockTreesIn;
    outpostLocation = outpostLocationIn;
}

TerrainChunkSetup::TerrainChunkSetup(TerrainChunkSetup &other){
    if(this != &other){
        *this = other;
    }
}

TerrainChunkSetup &TerrainChunkSetup::operator=(TerrainChunkSetup &other){
    if(this != &other){
        map2D = other.map2D;
        savedTerrainType = other.savedTerrainType;
        createOutpost = other.createOutpost;
        blockTrees = other.blockTrees;
    }
    return *this;
}

TerrainChunkSetup::~TerrainChunkSetup(){
    map2D = nullptr;
}


float TerrainChunkSetup::treeDensitySkalar(){
    if(createTrees()){
        /*
        EForest,
        ETropical,
        EDesert,
        EDesertForest,
        EOcean,
        ESnowHill
        */
        // dont go beyond 0.1 
        float scalar = 0.1f;
        if(savedTerrainType == ETerrainType::EForest) scalar = 0.1f;
        if(savedTerrainType == ETerrainType::ETropical) scalar = 0.1f;
        if(savedTerrainType == ETerrainType::EDesert) scalar = 0.05f;
        if(savedTerrainType == ETerrainType::EDesertForest) scalar = 0.1f;
        if(savedTerrainType == ETerrainType::EOcean) scalar = 0.05f;
        if(savedTerrainType == ETerrainType::ESnowHill) scalar = 0.05f;

        //dont use raw number which is for a 100% valid position part
        float scaledCorrectly = scalar * scaleUpFractionByLeftOverValidPositions();
        DebugHelper::logMessage("terrain tree fraction scaled up done: ", (float) scaledCorrectly);
        return scaledCorrectly;
    }
    return 0.0f;
}

float TerrainChunkSetup::scaleUpFractionByLeftOverValidPositions(){
    //left / all wäre z.b. 1/2, mal percent ist weniger, nicht gewollt,
    //part soll höher skalliert werden um auf gleiche percent zu kommen effektiv
    //also: mulitply by: all / left
    float left = freeFoliagePositions.Num();
    if(left <= 0.0f){
        return 0.0f;
    }
    if(map2D == nullptr){
        return 0.0f;
    }
    float all = map2D->size() * map2D->size();
    float scaledUp = all / left; 
    DebugHelper::logMessage("terrain tree fraction scaled up: ", (float) scaledUp);

    return scaledUp;
}



std::vector<std::vector<FVector>> &TerrainChunkSetup::mapReference(){
    if(map2D != nullptr){
        return *map2D;
    }
    return none;
}

/// @brief reference must NOT be TEMPORARY, STORED AS POINTER! 
/// @param refIn
void TerrainChunkSetup::setMapReference(std::vector<std::vector<FVector>> &refIn){
    if(&refIn != nullptr){
        map2D = &refIn;
    }
}


bool TerrainChunkSetup::createTrees(){
    return !blockTrees;
}

ETerrainType TerrainChunkSetup::getTerrainType(){
    return savedTerrainType;
}



void TerrainChunkSetup::createOutPostIfFlagged(UWorld *world){
    if(world && createOutpost){
        //might not work when using Plugin! - nothing will happen.
        worldLevelBase::addOutpostAt(outpostLocation);
    }
}

/// @brief free foliage positions by reference, without any normal dir filetring
/// @return free foliage positions reference
TArray<FVectorTouple> &TerrainChunkSetup::freeFoliagePositionsRef(){
    return freeFoliagePositions;
}
