#include "chunk.h"
#include "terrainConstants.h"


/***
 * 
 * ---- CHUNK METHODS -----
 * 
 */

chunk::chunk(int xPos, int yPos)
{
    savedTerrainType = ETerrainType::ETropical;

    setTreesBlocked(false);
    x = xPos;
    y = yPos;

    //creates the map for the chunk
    int limit = terrainConstants::CHUNKSIZE + 1; 
    //+1 fixes the gap to connect to other chunk, is closed and overriden
    //in the read and merge method!
    //do not change, this is correct.

    for (int i = 0; i < limit; i++){ //x
        
        std::vector<FVector> vec;
        std::vector<bool> placableFlags;
        for (int j = 0; j < limit; j++){ //y
            FVector pos(
                i * terrainConstants::ONEMETER, 
                j * terrainConstants::ONEMETER, 
                0.0f
            );
            vec.push_back(pos);
            placableFlags.push_back(true); //fill all positions ok
        }
        innerMap.push_back(vec);
        innerMapFreePositions.push_back(placableFlags);
    }
}

chunk::~chunk()
{
    for (int i = 0; i < innerMap.size(); i++){
        innerMap.at(i).clear();
    }
    innerMap.clear();
}

// ---- chunk methods ----
TerrainChunkSetup chunk::makeSetupPackage(){
    FVector locationWorld = position();

    TerrainChunkSetup package(
        innerMap,
        savedTerrainType,
        createOutpost,
        locationWorld,
        blockTrees,
        createBuilding
    );

    //copy free foliage positions
    freePositionsForFoliageLocal(
        package.freeFoliagePositionsRef()
    );


    return package;
}


void chunk::freePositionsForFoliageLocal( //in world space
    TArray<FVectorTouple> &outpositions
){
    for(int i = 0; i < innerMapFreePositions.size(); i++){ //x
        for(int j = 0; j < innerMapFreePositions[i].size(); j++){ //y
            if(indexFreeForFoliage(i,j)){
                if(xIsValid(i) && yIsValid(j)){
                    //FVectorTouple(FVector aIn, FVector bIn);
                    FVectorTouple touple(
                        innerMap[i][j], //position
                        normalFor(i,j) //normal
                    );
                    outpositions.Add(touple);
                }
            }
        }
    }
}


TerrainChunkSetup chunk::makeSetupPackage(
    chunk *top,
    chunk *right,
    chunk *topRight
){
    readAndMerge(top, right, topRight);
    return makeSetupPackage();
}

void chunk::setWasCreatedTrue(){
    wasCreated = true;
}
bool chunk::wasAlreadyCreated(){
    return wasCreated;
}

void chunk::setTreesBlocked(bool b){
    blockTrees = b;
}
bool chunk::createTrees(){
    return !blockTrees;
}

void chunk::markCreateOutpostTrue(){
    createOutpost = true;
}

void chunk::markBuildingCreateTrueAndBlockTrees(){
    createBuilding = true;
    setTreesBlocked(true);
}

/// @brief will return the inner map as reference, is not deisnged to be modified
/// @return map by reference, do not modify
std::vector<std::vector<FVector>> &chunk::readMap(){
    return innerMap;
}


void chunk::Merge(
    chunk *top,
    chunk *right,
    chunk *topRight
){
    readAndMerge(top, right, topRight);
}

/// @brief returns a deep copy of this chunks map connecting too 
/// @param top top chunk 
/// @param right right chunk
/// @param topRight top right chunk
/// @return right top and corner vertecy height is adjusted to match the other starting chunks
std::vector<std::vector<FVector>>& chunk::readAndMerge(
    chunk *top,
    chunk *right,
    chunk *topRight
){


    if(right != nullptr){
        //column to be overriden from
        std::vector<FVector> firstColFromNextRight = right->readFirstXColumn();
        for (int i = 0; i < firstColFromNextRight.size(); i++){
            //override 
            float currentVecHeight = firstColFromNextRight.at(i).Z;
            innerMap.at(innerMap.size() - 1).at(i).Z = currentVecHeight;
        }

        std::vector<bool> firstColFromNextRightBools = right->readFirstXColumnFoliage();
        for (int i = 0; i < firstColFromNextRightBools.size(); i++){
            int xCurrent = innerMap.size() - 1;
            int yCurrent = i;
            lockPositionForAnyFoliageIfTrue(xCurrent, yCurrent, firstColFromNextRightBools[i]);
        }
    }
    
    //might be wrong! (no its not.)
    if(top != nullptr){
        std::vector<FVector> firstRowFromNextTop = top->readFirstYRow();
        //row to be overriden from
        for (int j = 0; j < firstRowFromNextTop.size(); j++){
            if(j < innerMap.size()){
                //override
                FVector current = firstRowFromNextTop.at(j);
                innerMap.at(j).at(innerMap.size() - 1).Z = current.Z;
            }
        }

        std::vector<bool> firstRowFromNextTopFoliage = top->readFirstYRowFoliage();
        for (int i = 0; i < firstRowFromNextTopFoliage.size(); i++){
            int yCurrent = innerMap.size() - 1;
            int xCurrent = i;
            lockPositionForAnyFoliageIfTrue(xCurrent, yCurrent, firstRowFromNextTopFoliage[i]);
        }
    }


    if(topRight != nullptr){
        FVector topRightCorner = topRight->readBottomLeftCorner();
        
        //override
        innerMap.at(innerMap.size() - 1).at(innerMap.size() - 1).Z = topRightCorner.Z;

        //override foliage
        bool topRightFoliageFlag = topRight->readBottomLeftCornerFoliage();
        lockPositionForAnyFoliageIfTrue(
            innerMap.size() - 1, 
            innerMap.size() - 1, 
            topRightFoliageFlag
        );
    }

    return innerMap;
}


/**
 * RAYCAST
 */

//New
bool chunk::NextWorldVertexAt(FVector &a, FVector &out){

    // --- testing needed ---
    if(true || isInBounds(a)){
        
        int xa = 0;
        int ya = 0;
        convertPositionToInnerIndexClamped(a, xa, ya); //testing needed

        out = innerMap.at(xa).at(ya) + position();
        return true;
    }
    return false;
}




/// @brief adds a value to all positions of the chunk
/// @param value adds a value to the z part of each vertex in this chunk
void chunk::addheightForAll(int value){
    for (int i = 0; i < innerMap.size(); i++){
        for (int j = 0; j < innerMap.at(i).size(); j++){
            FVector &adjust = innerMap.at(i).at(j);
            adjust.Z += value;

            if(adjust.Z > terrainConstants::MAXHEIGHT){
                adjust.Z = terrainConstants::MAXHEIGHT;
            }
        }
    }
}

/// @brief multiplies the value to all positions of the chunk in z height
/// @param value mulitplicator
void chunk::scaleheightForAll(float value){
    for (int i = 0; i < innerMap.size(); i++){
        for (int j = 0; j < innerMap.at(i).size(); j++){
            FVector &adjust = innerMap.at(i).at(j);
            adjust.Z *= value;

            if(adjust.Z > terrainConstants::MAXHEIGHT){
                adjust.Z = terrainConstants::MAXHEIGHT;
            }
        }
    }
}

/// @brief sets the height for all positions of the chunk to a given value, overrides
/// @param value value to set
void chunk::setheightForAll(float value){
    for (int i = 0; i < innerMap.size(); i++)
    {
        for (int j = 0; j < innerMap.at(i).size(); j++){
            //du musst hier eine referenz erzeugen weil er sonst nicht reinschreibt?
            FVector &ref = innerMap.at(i).at(j);
            ref.Z = value;
        }
    }
}


void chunk::setheightForAllToAverage(){
    setheightForAll(heightAverage());
}

void chunk::clampheightForAllUpperLimitByOwnAverageHeight(){
    clampheightForAllUpperLimit(heightAverage());
}

void chunk::clampheightForAllUpperLimit(float value){
    for (int i = 0; i < innerMap.size(); i++)
    {
        for (int j = 0; j < innerMap.at(i).size(); j++){
            //du musst hier eine referenz erzeugen weil er sonst nicht reinschreibt?
            FVector &ref = innerMap.at(i).at(j);
            if(ref.Z > value){
                ref.Z = value;
            }
        }
    }
}






//testing with reading first row and column for weird osset fix

/// @brief return the column where x is 0 and y is iterating
/// @return first x column (upward along y)
std::vector<FVector> chunk::readFirstXColumn(){
    std::vector<FVector> output;
    for (int i = 0; i < innerMap.size(); i++)
    {
        output.push_back(innerMap.at(0).at(i));
        //output.push_back(innerMap.at(0).at(i));
    }
    return output;
}

/// @brief return the row where y is 0 and x is iterating
/// @return first y row (right along x)
std::vector<FVector> chunk::readFirstYRow(){
    std::vector<FVector> output;
    for (int i = 0; i < innerMap.size(); i++)
    {
        output.push_back(innerMap.at(i).at(0));
        //output.push_back(innerMap.at(i).at(0));
    }
    return output;
}

FVector chunk::readBottomLeftCorner(){
    return innerMap.at(0).at(0);
}


std::vector<bool> chunk::readFirstXColumnFoliage(){
    std::vector<bool> output;
    for (int i = 0; i < innerMapFreePositions.size(); i++)
    {
        output.push_back(innerMapFreePositions.at(0).at(i));
        //output.push_back(innerMap.at(0).at(i));
    }
    return output;
}

/// @brief return the row where y is 0 and x is iterating
/// @return first y row (right along x)
std::vector<bool> chunk::readFirstYRowFoliage(){
    std::vector<bool> output;
    for (int i = 0; i < innerMapFreePositions.size(); i++)
    {
        output.push_back(innerMapFreePositions.at(i).at(0));
    }
    return output;
}

bool chunk::readBottomLeftCornerFoliage(){
    return innerMapFreePositions.at(0).at(0);
}








/// @brief gets the position in cm based on the complete chunk layout
/// @return position in cm
FVector chunk::position(){
    FVector v(
        xPositionInCm(),
        yPositionInCm(),
        0
    );

    return v;
}

FVector chunk::positionPivotBottomLeft(){
    FVector newPos = position();
    return newPos;
    //bricks all fake raycast, keep out!
    /*
    //fix offset to be anchor at bottom left and not center of the mesh,
    //so the coordinate in mesh (0,0) is (0,0) and not (terrainConstants::CHUNKSIZE /2, terrainConstants::CHUNKSIZE/2)
    float offsetCenter = terrainConstants::ONEMETER * (terrainConstants::CHUNKSIZE / 2);
    newPos.X -= offsetCenter;
    newPos.Y -= offsetCenter;
    return newPos;
    */
}

int chunk::xPositionInCm(){
    int meter = terrainConstants::ONEMETER;
    int chunkIncm = meter * terrainConstants::CHUNKSIZE;
    int finalvalue = chunkIncm * x;
    return finalvalue;
}
int chunk::yPositionInCm(){
    int meter = terrainConstants::ONEMETER;
    int chunkIncm = meter * terrainConstants::CHUNKSIZE;
    int finalvalue = chunkIncm * y;
    return finalvalue;
}

int chunk::clampInnerIndex(int a){
    if(a >= innerMap.size()){
        a = innerMap.size() - 1;
    }
    if(a < 0){
        a = 0;
    }
    return a;
}


/// @brief will return if the inner index is in map bounds
/// @param a 
/// @return 
bool chunk::xIsValid(int a){
    return (a >= 0) && (a < innerMap.size());
}
bool chunk::yIsValid(int a){
    //return (innerMap.size() > 0) && (a >= 0) && (a < innerMap.at(0).size());
    return xIsValid(a);
}


FVector2D chunk::getFirstXColumnAnchor(int xColumn){
    
    FVector2D anchor(
        yPositionInCm(),
        innerMap.at(0).at(0).Z
    );

    if(xColumn < innerMap.size()){
        anchor.Y = innerMap.at(xColumn).at(0).Z;
    }
    return anchor; 
}

FVector2D chunk::getFirstYRowAnchor(int yRow){
    
    FVector2D anchor(
        xPositionInCm(),
        innerMap.at(0).at(0).Z
    );

    if(yRow < innerMap.size()){
        anchor.Y = innerMap.at(0).at(yRow).Z;
    }
    return anchor; 
    
    
}






/// @brief converts a value which can be used inside the map as index
/// @param value in any cm world!
/// @return 
int chunk::convertToInnerIndex(int value){
    int total = terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER;
    //scale down
    int mod = value % total;
    int toIndex = mod / terrainConstants::ONEMETER;
    return toIndex;
}

/// @brief converts the Y value the calmped value for this chunk
/// @param a 
/// @return 
int chunk::clampOuterYIndex(FVector2D &a){
    int total = terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER;
    int yToCheck = a.Y;
    int lowerRange = y * total;
    int higherRange = lowerRange + total;
    //inside bounds
    if(yToCheck >= lowerRange && yToCheck <= higherRange){
        return convertToInnerIndex(yToCheck);
    }

    //clamp out of bounds
    if(yToCheck <= lowerRange){
        return 0;
    }
    if(yToCheck >= higherRange){
        return innerMap.size() - 1;
    }
    return 0;
}


/// @brief applies a new height for an individual vertext
/// @param xIn xpos local index
/// @param yIn ypos local index
/// @param newHeight new height to apply
/// @param override apply with override or average
void chunk::applyIndivualVertexIndexBased(
    int xIn,
    int yIn,
    float newHeight,
    bool override
){
    
    if(xIsValid(xIn) && yIsValid(yIn)){

        //override
        if(override){
            FVector copy = innerMap.at(xIn).at(yIn);
            copy.Z = newHeight;
            innerMap.at(xIn).at(yIn) = copy;
        }else{

            float newAvg = innerMap.at(xIn).at(yIn).Z;
            newAvg += newHeight;
            newAvg /= 2;
            
            if(newAvg > terrainConstants::MAXHEIGHT){
                newAvg = terrainConstants::MAXHEIGHT;
            }
            innerMap.at(xIn).at(yIn).Z = newAvg;
        }
    }
}

/// @brief will check if a WORLD position is within the chunks bounds
/// @param a position to check
/// @return return true if within the chunks index bounds on x and y axis
bool chunk::isInBounds(FVector &a){
    int total = terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER;
    int selfX = xPositionInCm();
    int selfY = yPositionInCm();
    int selfXUpper = selfX + total;
    int selfYUpper = selfY + total;

    return selfX <= a.X && a.X <= selfXUpper &&
        selfY <= a.Y && a.Y <= selfYUpper;
}


float chunk::heightAverage(){
    if(innerMap.size() == 0 || innerMap[0].size() == 0){
        return 0.0f;
    }

    float sum = 0.0f;
    for (int i = 0; i < innerMap.size(); i++)
    {
        for (int j = 0; j < innerMap[i].size(); j++){
            sum += innerMap[i][j].Z;
        }
    }
    int vertexCountAll = innerMap.size() * innerMap[0].size();
    if(vertexCountAll > 0){
        sum /= vertexCountAll;
    }
    return sum;
}

float chunk::maxHeight(){
    float output = 0.0f;
    for (int i = 0; i < innerMap.size(); i++){
        for (int j = 0; j < innerMap[i].size(); j++){
            float current = innerMap[i][j].Z;
            if(current > output){
                output = current;
            }
        }
    }
    return output;
}

float chunk::minHeight(){
    float output = terrainConstants::HEIGHT_MAX_OCEAN + 1; //above limit
    for (int i = 0; i < innerMap.size(); i++){
        for (int j = 0; j < innerMap[i].size(); j++){
            float current = innerMap[i][j].Z;
            if(current < output){
                output = current;
            }
        }
    }
    return output;
}


// --- chunk block area functions ---

//new precise bounds
void chunk::blockAreaForFoliage(
    FVector &v0,
    FVector &v1,
    FVector &v2,
    FVector &v3
){
    FVector removeOffset = position();
    FVector v0_ = v0 - removeOffset;
    FVector v1_ = v1 - removeOffset;
    FVector v2_ = v2 - removeOffset;
    FVector v3_ = v3 - removeOffset;

    BoundingShapeXY shape; //TESTING NEEDED!
    shape.initWithSortedQuad(v0_, v1_, v2_, v3_);

    int minX, minY, maxX, maxY = 0;
    /*
    1 -> 2
    |    |
    0 <- 3
    */
    generateBoundingIndicesFromWorldSpace(v0, v2, minX, minY, maxX, maxY);
    
    for(int i = minX; i <= maxX; i++){
        for(int j = minY; j <= maxY; j++){
            //lockPositionForAnyFoliage(i,j);
            int iCopy = clampInnerIndex(i);
            int jCopy = clampInnerIndex(j);
            if(xIsValid(iCopy) && yIsValid(jCopy)){
                FVector &vertex = innerMap[iCopy][jCopy];
                if(shape.isInsideShape(vertex)){
                    lockPositionForAnyFoliage(iCopy,jCopy);
                }
            }
        }
    }

}

//old simple bounds
void chunk::blockAreaForFoliage(
    FVector &a, 
    FVector &b
){

    int minX, minY, maxX, maxY = 0;
    generateBoundingIndicesFromWorldSpace(a, b, minX, minY, maxX, maxY);


    if (true)
    {
        FString message = FString::Printf(
            TEXT(
                "chunk::terrain blocked area: (%d, %d) (%d, %d)"
            ),
            minX, minY, maxX, maxY
        );
        DebugHelper::logMessage(message);
    }

    for(int i = minX; i <= maxX; i++){
        for(int j = minY; j <= maxY; j++){
            lockPositionForAnyFoliage(i,j);
        }
    }
}

void chunk::generateBoundingIndicesFromWorldSpace(
    FVector &a,
    FVector &b,
    int &minX,
    int &minY,
    int &maxX,
    int &maxY
){
    int ax = convertToInnerIndex(a.X);
    int ay = convertToInnerIndex(a.Y);
    int bx = convertToInnerIndex(b.X);
    int by = convertToInnerIndex(b.Y);

    //new - prüfung intesiver notwendig
    convertPositionToInnerIndexClamped(a, ax, ay);
    convertPositionToInnerIndexClamped(b, bx, by);

    minX = std::min(ax, bx);
    maxX = std::max(ax, bx);
    minY = std::min(ay, by);
    maxY = std::max(ay, by);
}



void chunk::convertPositionToInnerIndexClamped(
    FVector inpos,
    int &i,
    int &j
){
    inpos -= position();

    int xIndex = inpos.X / terrainConstants::ONEMETER;
    int yIndex = inpos.Y / terrainConstants::ONEMETER;

    xIndex = std::min(xIndex, int(innerMap.size() - 1));
    yIndex = std::min(yIndex, int(innerMap[0].size() - 1));
    
    
    
    //xIndex = std::min(xIndex, terrainConstants::CHUNKSIZE);
    //yIndex = std::min(yIndex, terrainConstants::CHUNKSIZE);
    xIndex = std::max(xIndex, 0);
    yIndex = std::max(yIndex, 0);
    
    i = xIndex;
    j = yIndex;

    /*
    i = convertToInnerIndex(inpos.X);
    j = convertToInnerIndex(inpos.Y);
    */
}


//is clamped!
void chunk::lockPositionForAnyFoliage(int i, int j){
    i = clampInnerIndex(i);
    j = clampInnerIndex(j);
    if(xIsValid(i) && yIsValid(j)){
        innerMapFreePositions[i][j] = false; //set to blocked
    }
}

void chunk::lockPositionForAnyFoliageIfTrue(int i, int j, bool flag){
    if(flag){
        lockPositionForAnyFoliage(i,j);
    }
}


bool chunk::indexFreeForFoliage(int i, int j){
    if(xIsValid(i) && yIsValid(j)){
        return innerMapFreePositions[i][j]; //true ok, otherwise false
    }
    return false;
}




FVector chunk::normalFor(int i, int j){
    if(xIsValid(i) && yIsValid(j) && xIsValid(i+1) && xIsValid(j+1)){
        /*
        ^y
        |
        1   2
        
        0   3 -> x
        */
        FVector &v0 = innerMap[i][j];
        FVector &v1 = innerMap[i][j+1];
        FVector &v2 = innerMap[i+1][j];

        FVector normal = FVector::CrossProduct(v1 - v0, v2 - v0);
        normal = normal.GetSafeNormal();
        return normal;
    }


    return FVector(0,0,1);
}




// --- chunk plotting functions ---

void chunk::plot(UWorld *world){

    FVector offset(
        x * terrainConstants::ONEMETER * terrainConstants::CHUNKSIZE, 
        y * terrainConstants::ONEMETER * terrainConstants::CHUNKSIZE, 
        0.0f
    );
    

    FColor currentColor = FColor::Yellow;

    bool a = x % 2 == 0;
    bool b = y % 2 == 0;
    if(a && b)
        currentColor = FColor::Yellow; 
    
    if(a && !b)
        currentColor = FColor::Green; 
    
    if(!a && b)
        currentColor = FColor::Red; 
    
    if(!a && !b)
        currentColor = FColor::Blue; 


    if(world != nullptr){
        for (int i = 1; i < innerMap.size(); i++){
            for (int j = 1; j < innerMap.at(i).size(); j++){

                FVector prevLeft = innerMap.at(i - 1).at(j) + offset;
                FVector prevDown = innerMap.at(i).at(j-1) + offset;
                FVector current = innerMap.at(i).at(j) + offset;

                DebugHelper::showLineBetween(world, prevLeft, current, currentColor);
                DebugHelper::showLineBetween(world, prevDown, current, currentColor);
            }
        }
    }
}


ETerrainType chunk::getTerrainType(){
    return savedTerrainType;
}
void chunk::updateTerraintype(ETerrainType typeIn){
    savedTerrainType = typeIn;
}

void chunk::updateTerrainTypeBySpecialHeights(){
    float currentHeightAverage = heightAverage();
    float currentMinHeight = minHeight();
    if(currentHeightAverage > terrainConstants::HEIGH_AVG_SNOWHILL_LOWERBOUND){
        updateTerraintype(ETerrainType::ESnowHill);
    }            
    
    if(currentMinHeight < terrainConstants::HEIGHT_MAX_OCEAN){
        updateTerraintype(ETerrainType::EOcean);
        DebugHelper::logMessage("terrain min height terrain: ", currentMinHeight);
    }
}
 