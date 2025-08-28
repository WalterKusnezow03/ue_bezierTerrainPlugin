#include "TerrainChunkMap.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/terrainHillSetup.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/terrainConstants.h"
#include "GameCore/util/FVectorUtil.h"

TerrainChunkMap::TerrainChunkMap(){
    insetHillData = std::abs(insetHillData);
}
TerrainChunkMap::~TerrainChunkMap(){

}

// --- Storage Interface ---
bool TerrainChunkMap::Load(FString worldLevelName){
    worldLevelNameSaved = worldLevelName;

    ChunkMapStorageInterface storageInterface;
    return storageInterface.Load(worldLevelNameSaved, *this);
}

void TerrainChunkMap::Save(){
    ChunkMapStorageInterface storageInterface;
    storageInterface.Save(worldLevelNameSaved, *this);
}




// --- Class methods ---

int TerrainChunkMap::countTotalCunks(){
    return getAll().Num();
}

int TerrainChunkMap::countPerAxis(){
    return chunks;
}

void TerrainChunkMap::createChunkMap(int chunksIn){
    verfiyChunks(chunksIn);
    createArray();
    createRandomHeightMap();
    createRandomOutpostFlags();
}

void TerrainChunkMap::verfiyChunks(int chunksIn){
    chunksIn = std::abs(chunksIn);
    chunks = std::max(chunksIn, 10);
}

void TerrainChunkMap::createArray(){
    innerMap.Empty(); //clean.

    innerMap.SetNum(chunks);
    for (int i = 0; i < innerMap.Num(); i++){
        innerMap[i] = TArray<FTerrainChunkAttributes>();
        TArray<FTerrainChunkAttributes> &ref = innerMap[i];
        ref.SetNum(chunks);
        for (int j = 0; j < ref.Num(); j++){
            FTerrainChunkAttributes &current = ref[j];
            current.posX = i;
            current.posY = j;
        }
    }
}

void TerrainChunkMap::createRandomHeightMap(){
    //jeder chunk soll z.b. im schnitt 20 erhöhungen bekommen.
    int layers = chunks * 3;
    createRandomHeightMapChunkWide(layers);
}

void TerrainChunkMap::createRandomHeightMapChunkWide(int layers){

    layers = std::abs(layers);
    for (int i = 0; i < layers; i++)
    {
        terrainHillSetup newHill = createRandomHillData();
        applyHillData(newHill);
    }

    if(terrainConstants::MINCHUNK_LARGEHILL < chunks){
        int layersLarge = layers / 4.0f;
        for (int i = 0; i < layersLarge; i++){
            terrainHillSetup newHillLarge = createRandomHillDataLargeScale();
            applyHillData(newHillLarge);
        }
    }
}







terrainHillSetup TerrainChunkMap::createRandomHillData(){
    int scaleX = FVectorUtil::randomNumber(terrainConstants::MINCHUNK_HILL, chunks); //random hardcoded for now.
    int scaleY = FVectorUtil::randomNumber(terrainConstants::MINCHUNK_HILL, chunks);
    int heightMin = terrainConstants::ONEMETER / 2.0f;
    return createRandomHillData(scaleX, scaleY,heightMin);
}

terrainHillSetup TerrainChunkMap::createRandomHillDataLargeScale(){
    
    int scaleX = FVectorUtil::randomNumber(terrainConstants::MINCHUNK_LARGEHILL, chunks); //random hardcoded for now.
    int scaleY = FVectorUtil::randomNumber(terrainConstants::MINCHUNK_LARGEHILL, chunks);
    int heightMin = terrainConstants::ONEMETER * 2.0f;
    return createRandomHillData(scaleX, scaleY, heightMin);
}

terrainHillSetup TerrainChunkMap::createRandomHillData(
    int sizeX, 
    int sizeY,
    int heightMin
){
    sizeX = std::abs(sizeX);
    sizeY = std::abs(sizeY);
    if(sizeX <= 0){
        sizeX = 1;
    }
    if(sizeY <= 0){
        sizeY = 1;
    }


    int startX = clampIndex(FVectorUtil::randomNumber(1, chunks - sizeX));
    int startY = clampIndex(FVectorUtil::randomNumber(1, chunks - sizeY));
    int heightMax = heightMin * 3; //2

    return terrainHillSetup(
        startX,
        startY,
        sizeX,
        sizeY,
        heightMin,
        heightMax
    );
}

void TerrainChunkMap::applyHillData(std::vector<terrainHillSetup> &hillDataVec){
    for (int i = 0; i < hillDataVec.size(); i++){
        applyHillData(hillDataVec[i]);
    }
}


/// @brief will enheight the map based on the passed hilldata in size X, size Y and height add
/// @param hillData 
void TerrainChunkMap::applyHillData(terrainHillSetup &hillData){
    //clamped to index inset.
    for (int i = clampIndexInset(hillData.xPosCopy()); i < clampIndexInset(hillData.xTargetCopy()); i++){
        for (int j = clampIndexInset(hillData.yPosCopy()); j < clampIndexInset(hillData.yTargetCopy()); j++){
            FTerrainChunkAttributes &ref = find(i, j);
            ref.height += hillData.getHeightIfSetOrRandomHeight();
        }
    }
}


void TerrainChunkMap::createRandomOutpostFlags(){
    int max = chunks / 5;
    for (int i = 0; i < max; i++){
        createRandomOutpostFlagAndSmoothArea();
    }
}

void TerrainChunkMap::createRandomOutpostFlagAndSmoothArea(){
    int i = FVectorUtil::randomNumber(insetHillData, chunks - insetHillData);
    int j = FVectorUtil::randomNumber(insetHillData, chunks - insetHillData);
    FTerrainChunkAttributes &attributes = find(i, j);

    //flag outpost
    attributes.outpostFlagged = true;

    //set height same around area
    int bound = 1;
    TArray<FTerrainChunkAttributes *> quadToEvenHeight = getQuad(
        i - bound, 
        j - bound, 
        bound * 2 + 1 //extent to top corner
    );
    if(quadToEvenHeight.Num() <= 0){
        return;
    }

    //find average height and flag building needed
    float heightTotal = 0.0f;
    for (int c = 0; c < quadToEvenHeight.Num(); c++)
    {
        FTerrainChunkAttributes *currentAttributes = quadToEvenHeight[c];
        if(currentAttributes){
            heightTotal += currentAttributes->height;

            //flag building needed around outpost.
            currentAttributes->buildingFlagged = true;
        }
    }

    //override height
    float heightAverageToSet = heightTotal / quadToEvenHeight.Num();
    for (int c = 0; c < quadToEvenHeight.Num(); c++)
    {
        FTerrainChunkAttributes *currentAttributes = quadToEvenHeight[c];
        if(currentAttributes){
            currentAttributes->height = heightAverageToSet;
        }
    }
}

FTerrainChunkAttributes &TerrainChunkMap::find(int i, int j){
    verifyIndex(i);
    verifyIndex(j);
    return innerMap[i][j];
}

void TerrainChunkMap::override(FTerrainChunkAttributes &attributes, int i, int j){
    verifyIndex(i);
    verifyIndex(j);
    innerMap[i][j] = attributes;
}

/// @brief makes index VALID IN BOUNDS
/// @param index 
void TerrainChunkMap::verifyIndex(int &index){
    index = std::max(index, 0);
    index = std::min(innerMap.Num() - 1, index);
}


int TerrainChunkMap::clampIndex(int index){
    verifyIndex(index);
    return index;
}

bool TerrainChunkMap::indexIsValid(int i, int j){
    //is okay to check because false result is there before oob idnex would be reached
    return i >= 0 && j >= 0 &&
           i < innerMap.Num() && j < innerMap[i].Num();
}

int TerrainChunkMap::clampIndexInset(int index){
    index = std::max(index, insetHillData);
    index = std::min(index, chunks - 1 - insetHillData);
    return index;
}

// ---- api for terrain generator ----
TArray<FTerrainChunkAttributes *> TerrainChunkMap::getQuad(
    int x, 
    int y, 
    int chunksToCopy
){
    int xEnd = x + chunksToCopy + 1;//becaus verify index capped at size -1 
    int yEnd = y + chunksToCopy + 1;
    verifyIndex(x);
    verifyIndex(y);
    verifyIndex(xEnd); //makes valid in bounds
    verifyIndex(yEnd);

    TArray<FTerrainChunkAttributes *> outArray;
    for (int i = x; i <= xEnd; i++)
    {
        for (int j = y; j <= yEnd; j++){
            if(indexIsValid(i,j)){
                FTerrainChunkAttributes &ref = find(i, j);
                outArray.Add(&ref);
            }
            
        }
    }
    return outArray;
}




// ---- api storage interface -----
TArray<FTerrainChunkAttributes *> TerrainChunkMap::getAll(){
    return getQuad(0, 0, chunks);
}

void TerrainChunkMap::constructQuadFrom(TArray<FTerrainChunkAttributes> &data){
    if(data.Num() > 0){
        int sizeEdge = std::sqrt(data.Num());

        //init map from loaded data, chunk number saved (axis)
        createChunkMap(sizeEdge);

        int arrayIndex = 0; //projected to 1D
        for (int i = 0; i < sizeEdge; i++)
        {
            for (int j = 0; j < sizeEdge; j++){
                FTerrainChunkAttributes &current = data[arrayIndex];
                override(current, i, j);
                arrayIndex++;
            }
        }
    }
}



FString TerrainChunkMap::toString(){
    TArray<FTerrainChunkAttributes *> chunksArray = getAll();
    FString outString = TEXT("TerrainChunkMap: ToString: ");
    for (int i = 0; i < chunksArray.Num(); i++){
        FTerrainChunkAttributes *current = chunksArray[i];
        if(current){
            outString += FString::Printf(TEXT("(%d %d %d)"), current->posX, current->posY, current->height);
        }
    }
    return outString;
}







