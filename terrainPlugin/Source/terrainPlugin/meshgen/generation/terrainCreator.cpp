// Fill out your copyright notice in the Description page of Project Settings.

#include <cmath>
#include "GameCore/util/FVectorUtil.h"
#include "Algo/Sort.h"  // Include the necessary header
#include "bezierCurve.h"
#include "GameCore/util/TVector.h"
#include "HAL/PlatformTime.h"
#include <algorithm>
#include <set>
#include "GameCore/MeshGenBase/foliage/ETerrainType.h"
#include "CoreMath/Matrix/MMatrix.h"

#include "GameCore/EntityGC/EntityManagerBase.h"
#include "GameCore/MeshGenBase/MeshData/BoundingBox/BoundingShapeXY.h"

#include "terrainPlugin/meshgen/rooms/roomActor/roomProcedural.h"
#include "terrainPlugin/meshgen/water/customWaterActor.h"
#include "terrainPlugin/meshgen/foliage/helper/FVectorShape.h"

#include "terrainCreator.h"

terrainCreator::terrainCreator()
{
}

terrainCreator::~terrainCreator()
{

}


/***
 * 
 * ---- CHUNK METHODS -----
 * 
 */

terrainCreator::chunk::chunk(int xPos, int yPos)
{
    savedTerrainType = ETerrainType::ETropical;

    setTreesBlocked(false);
    x = xPos;
    y = yPos;

    //creates the map for the chunk
    int limit = terrainCreator::CHUNKSIZE + 1; 
    //+1 fixes the gap to connect to other chunk, is closed and overriden
    //in the read and merge method!
    //do not change, this is correct.

    for (int i = 0; i < limit; i++){ //x
        
        std::vector<FVector> vec;
        std::vector<bool> placableFlags;
        for (int j = 0; j < limit; j++){ //y
            FVector pos(
                i * terrainCreator::ONEMETER, 
                j * terrainCreator::ONEMETER, 
                0.0f
            );
            vec.push_back(pos);
            placableFlags.push_back(true); //fill all positions ok
        }
        innerMap.push_back(vec);
        innerMapFreePositions.push_back(placableFlags);
    }
}

terrainCreator::chunk::~chunk()
{
    for (int i = 0; i < innerMap.size(); i++){
        innerMap.at(i).clear();
    }
    innerMap.clear();
}

// ---- chunk methods ----
TerrainChunkSetup terrainCreator::chunk::makeSetupPackage(){
    FVector locationWorld = position();

    TerrainChunkSetup package(
        innerMap,
        savedTerrainType,
        createOutpost,
        locationWorld,
        blockTrees
    );

    //copy free foliage positions
    freePositionsForFoliageLocal(
        package.freeFoliagePositionsRef()
    );


    return package;
}

TerrainChunkSetup terrainCreator::chunk::makeSetupPackage(
    chunk *top,
    chunk *right,
    chunk *topRight
){
    readAndMerge(top, right, topRight);
    return makeSetupPackage();
}

void terrainCreator::chunk::setWasCreatedTrue(){
    wasCreated = true;
}
bool terrainCreator::chunk::wasAlreadyCreated(){
    return wasCreated;
}

void terrainCreator::chunk::setTreesBlocked(bool b){
    blockTrees = b;
}
bool terrainCreator::chunk::createTrees(){
    return !blockTrees;
}

void terrainCreator::chunk::markCreateOutpostTrue(){
    createOutpost = true;
}




/// @brief will return the inner map as reference, is not deisnged to be modified
/// @return map by reference, do not modify
std::vector<std::vector<FVector>> &terrainCreator::chunk::readMap(){
    return innerMap;
}

/// @brief returns a deep copy of this chunks map connecting too 
/// @param top top chunk 
/// @param right right chunk
/// @param topRight top right chunk
/// @return right top and corner vertecy height is adjusted to match the other starting chunks
std::vector<std::vector<FVector>>& terrainCreator::chunk::readAndMerge(
    terrainCreator::chunk *top,
    terrainCreator::chunk *right,
    terrainCreator::chunk *topRight
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
/// @brief get the height for an specific vertex
/// @param a vertex
/// @return 
float terrainCreator::chunk::getHeightFor(FVector &a){
    if(isInBounds(a)){
        
        //int xa = convertToInnerIndex(a.X);
        //int ya = convertToInnerIndex(a.Y);

        int xa = 0;
        int ya = 0;
        convertPositionToInnerIndexClamped(a, xa, ya); //testing needed

        return innerMap.at(xa).at(ya).Z;
    }
    return a.Z;
}

void terrainCreator::chunk::getHeightWith(
    HeightExtractionData &data
){
    FVector &positionWorld = data.vertexPositionToLookFor();
    if (isInBounds(positionWorld))
    {
        int xa = 0;
        int ya = 0;
        convertPositionToInnerIndexClamped(positionWorld, xa, ya);

        FVector &innerMapVertex = innerMap.at(xa).at(ya);
        FVector vertexWorldSpace = innerMapVertex + position();
        data.SetupDataAndWeighting(vertexWorldSpace);

        //debug
        FString s = FString::Printf(
            TEXT("terrain vertex (%d, %d) at %.2f   xy(%.2f, %.2f)"),
            xa,
            ya,
            vertexWorldSpace.Z,
            vertexWorldSpace.X,
            vertexWorldSpace.Y
        );
        
        if(false)
            DebugHelper::showScreenMessage(s);
    }
}






/// @brief adds a value to all positions of the chunk
/// @param value adds a value to the z part of each vertex in this chunk
void terrainCreator::chunk::addheightForAll(int value){
    for (int i = 0; i < innerMap.size(); i++){
        for (int j = 0; j < innerMap.at(i).size(); j++){
            FVector &adjust = innerMap.at(i).at(j);
            adjust.Z += value;

            if(adjust.Z > terrainCreator::MAXHEIGHT){
                adjust.Z = terrainCreator::MAXHEIGHT;
            }
        }
    }
}

/// @brief multiplies the value to all positions of the chunk in z height
/// @param value mulitplicator
void terrainCreator::chunk::scaleheightForAll(float value){
    for (int i = 0; i < innerMap.size(); i++){
        for (int j = 0; j < innerMap.at(i).size(); j++){
            FVector &adjust = innerMap.at(i).at(j);
            adjust.Z *= value;

            if(adjust.Z > terrainCreator::MAXHEIGHT){
                adjust.Z = terrainCreator::MAXHEIGHT;
            }
        }
    }
}

/// @brief sets the height for all positions of the chunk to a given value, overrides
/// @param value value to set
void terrainCreator::chunk::setheightForAll(float value){
    for (int i = 0; i < innerMap.size(); i++)
    {
        for (int j = 0; j < innerMap.at(i).size(); j++){
            //du musst hier eine referenz erzeugen weil er sonst nicht reinschreibt?
            FVector &ref = innerMap.at(i).at(j);
            ref.Z = value;
        }
    }
}


void terrainCreator::chunk::setheightForAllToAverage(){
    setheightForAll(heightAverage());
}

void terrainCreator::chunk::clampheightForAllUpperLimitByOwnAverageHeight(){
    clampheightForAllUpperLimit(heightAverage());
}

void terrainCreator::chunk::clampheightForAllUpperLimit(float value){
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
std::vector<FVector> terrainCreator::chunk::readFirstXColumn(){
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
std::vector<FVector> terrainCreator::chunk::readFirstYRow(){
    std::vector<FVector> output;
    for (int i = 0; i < innerMap.size(); i++)
    {
        output.push_back(innerMap.at(i).at(0));
        //output.push_back(innerMap.at(i).at(0));
    }
    return output;
}

FVector terrainCreator::chunk::readBottomLeftCorner(){
    return innerMap.at(0).at(0);
}


std::vector<bool> terrainCreator::chunk::readFirstXColumnFoliage(){
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
std::vector<bool> terrainCreator::chunk::readFirstYRowFoliage(){
    std::vector<bool> output;
    for (int i = 0; i < innerMapFreePositions.size(); i++)
    {
        output.push_back(innerMapFreePositions.at(i).at(0));
    }
    return output;
}

bool terrainCreator::chunk::readBottomLeftCornerFoliage(){
    return innerMapFreePositions.at(0).at(0);
}








/// @brief gets the position in cm based on the complete chunk layout
/// @return position in cm
FVector terrainCreator::chunk::position(){
    FVector v(
        xPositionInCm(),
        yPositionInCm(),
        0
    );

    return v;
}

FVector terrainCreator::chunk::positionPivotBottomLeft(){
    FVector newPos = position();
    return newPos;
    //bricks all fake raycast, keep out!
    /*
    //fix offset to be anchor at bottom left and not center of the mesh,
    //so the coordinate in mesh (0,0) is (0,0) and not (chunksize /2, chunksize/2)
    float offsetCenter = terrainCreator::ONEMETER * (terrainCreator::CHUNKSIZE / 2);
    newPos.X -= offsetCenter;
    newPos.Y -= offsetCenter;
    return newPos;
    */
}

int terrainCreator::chunk::xPositionInCm(){
    int meter = terrainCreator::ONEMETER;
    int chunksize = terrainCreator::CHUNKSIZE;
    int chunkIncm = meter * chunksize;
    int finalvalue = chunkIncm * x;
    return finalvalue;
}
int terrainCreator::chunk::yPositionInCm(){
    int meter = terrainCreator::ONEMETER;
    int chunksize = terrainCreator::CHUNKSIZE;
    int chunkIncm = meter * chunksize;
    int finalvalue = chunkIncm * y;
    return finalvalue;
}

int terrainCreator::chunk::clampInnerIndex(int a){
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
bool terrainCreator::chunk::xIsValid(int a){
    return (a >= 0) && (a < innerMap.size());
}
bool terrainCreator::chunk::yIsValid(int a){
    //return (innerMap.size() > 0) && (a >= 0) && (a < innerMap.at(0).size());
    return xIsValid(a);
}


FVector2D terrainCreator::chunk::getFirstXColumnAnchor(int xColumn){
    
    FVector2D anchor(
        yPositionInCm(),
        innerMap.at(0).at(0).Z
    );

    if(xColumn < innerMap.size()){
        anchor.Y = innerMap.at(xColumn).at(0).Z;
    }
    return anchor; 
}

FVector2D terrainCreator::chunk::getFirstYRowAnchor(int yRow){
    
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
int terrainCreator::chunk::convertToInnerIndex(int value){
    int total = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;
    //scale down
    int mod = value % total;
    int toIndex = mod / terrainCreator::ONEMETER;
    return toIndex;
}

/// @brief converts the Y value the calmped value for this chunk
/// @param a 
/// @return 
int terrainCreator::chunk::clampOuterYIndex(FVector2D &a){
    int total = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;
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
void terrainCreator::chunk::applyIndivualVertexIndexBased(
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
            
            if(newAvg > terrainCreator::MAXHEIGHT){
                newAvg = terrainCreator::MAXHEIGHT;
            }
            innerMap.at(xIn).at(yIn).Z = newAvg;
        }
    }
}

/// @brief will check if a WORLD position is within the chunks bounds
/// @param a position to check
/// @return return true if within the chunks index bounds on x and y axis
bool terrainCreator::chunk::isInBounds(FVector &a){
    int total = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;
    int selfX = xPositionInCm();
    int selfY = yPositionInCm();
    int selfXUpper = selfX + total;
    int selfYUpper = selfY + total;

    return selfX <= a.X && a.X <= selfXUpper &&
           selfY <= a.Y && a.Y <= selfYUpper;
}


float terrainCreator::chunk::heightAverage(){
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

float terrainCreator::chunk::maxHeight(){
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

float terrainCreator::chunk::minHeight(){
    float output = HEIGHT_MAX_OCEAN + 1; //above limit
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
void terrainCreator::chunk::blockAreaForFoliage(
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
    generateBoundingIndicesFromWorldSpace(v0_, v2_, minX, minY, maxX, maxY);
    
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
void terrainCreator::chunk::blockAreaForFoliage(
    FVector &a, 
    FVector &b
){

    int minX, minY, maxX, maxY = 0;
    generateBoundingIndicesFromWorldSpace(a, b, minX, minY, maxX, maxY);


    if (false)
    {
        FString message = FString::Printf(
            TEXT(
                "terrain blocked area: (%d, %d) (%d, %d)"
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

void terrainCreator::chunk::generateBoundingIndicesFromWorldSpace(
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



void terrainCreator::chunk::convertPositionToInnerIndexClamped(
    FVector inpos,
    int &i,
    int &j
){
    inpos -= position();

    int xIndex = inpos.X / terrainCreator::ONEMETER;
    int yIndex = inpos.Y / terrainCreator::ONEMETER;

    xIndex = std::min(xIndex, terrainCreator::CHUNKSIZE);
    yIndex = std::min(yIndex, terrainCreator::CHUNKSIZE);
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
void terrainCreator::chunk::lockPositionForAnyFoliage(int i, int j){
    i = clampInnerIndex(i);
    j = clampInnerIndex(j);
    if(xIsValid(i) && yIsValid(j)){
        innerMapFreePositions[i][j] = false; //set to blocked
    }
}

void terrainCreator::chunk::lockPositionForAnyFoliageIfTrue(int i, int j, bool flag){
    if(flag){
        lockPositionForAnyFoliage(i,j);
    }
}


bool terrainCreator::chunk::indexFreeForFoliage(int i, int j){
    if(xIsValid(i) && yIsValid(j)){
        return innerMapFreePositions[i][j]; //true ok, otherwise false
    }
    return false;
}

void terrainCreator::chunk::freePositionsForFoliageLocal( //in world space
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



FVector terrainCreator::chunk::normalFor(int i, int j){
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

void terrainCreator::chunk::plot(UWorld *world){

    FVector offset(
        x * terrainCreator::ONEMETER * terrainCreator::CHUNKSIZE, 
        y * terrainCreator::ONEMETER * terrainCreator::CHUNKSIZE, 
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


ETerrainType terrainCreator::chunk::getTerrainType(){
    return savedTerrainType;
}
void terrainCreator::chunk::updateTerraintype(ETerrainType typeIn){
    savedTerrainType = typeIn;
}

void terrainCreator::chunk::updateTerrainTypeBySpecialHeights(){
    float currentHeightAverage = heightAverage();
    float currentMinHeight = minHeight();
    if(currentHeightAverage > HEIGH_AVG_SNOWHILL_LOWERBOUND){
        updateTerraintype(ETerrainType::ESnowHill);
    }            
    
    if(currentMinHeight < HEIGHT_MAX_OCEAN){
        updateTerraintype(ETerrainType::EOcean);
        DebugHelper::logMessage("terrain min height terrain: ", currentMinHeight);
    }
}



/***
 * 
 * ---- TERRAIN METHODS -----
 * 
 */

/// @brief will always create a qudratic terrain
/// @param world world to spawn in / debug draw in
/// @param meters size in meters
void terrainCreator::createTerrain(UWorld *world, int meters){
    std::vector<terrainHillSetup> none;
    createTerrain(world, meters, none);
}

/// @brief will always create a qudratic terrain
/// @param world world to spawn in / debug draw in
/// @param meters size in meters
/// @param predefinedHillDataVecFlatArea chunks to keep a certain height forced, may vary because of
/// bezier smoothening.
void terrainCreator::createTerrain(
    UWorld *world,
    int meters,
    std::vector<terrainHillSetup> &predefinedHillDataVecFlatArea //flat area
){
    worldPointer = world;

    int chunks = floor(meters / terrainCreator::CHUNKSIZE); //to chunks
    //int detail = CHUNKSIZE; // 1 by 1 detail

    //fill map
    map.reserve(chunks);
    for (int i = 0; i < chunks; i++){
        std::vector<terrainCreator::chunk> vec;
        vec.reserve(chunks);
        for (int j = 0; j < chunks; j++){
            chunk c(i,j);
            vec.push_back(c);
        }
        map.push_back(vec);
    }


    //random height and smooth
    int layers = 12; //20 (12 waren auf 10x10)

    int layersPerTen = 12;
    layers = (chunks / 10.0f) * layersPerTen;

    createRandomHeightMapChunkWide(layers);
    smooth3dMap();


    flattenChunksForHillData(predefinedHillDataVecFlatArea); //override after smooth height, clamp upper limit
}











/// @brief scales the height for all chunks (designed to upscale before bezier and downscale later)
/// creates more detailed interpolation on Z axis (maybe)
/// @param scale sclae to set
void terrainCreator::scaleHeightForAll(float scale){
    for (int i = 0; i < map.size(); i++){
        for (int j = 0; j < map.at(i).size(); j++){
            map.at(i).at(j).scaleheightForAll(scale);
        }
    }
}





/**
 * ----- 3D SMOOTH MAP SECTION -----
 */

/// @brief will smooth out all chunks rows and columns and merge them together to the map
void terrainCreator::smooth3dMap(){
    FVector a(0, 0, 0);
    int max = map.size() * terrainCreator::ONEMETER * terrainCreator::CHUNKSIZE;
    FVector b(max, max, 0);

    int iterations = 3;
    smooth3dMap(a, b, iterations);
    //smooth3dMap(a, b, 1);
}

/// @brief will smooth out all chunks rows and columns and merge them together to the map
void terrainCreator::smooth3dMap(FVector &a, FVector &b, int iterations){

    //calculate enclosed bounds, works as expected
    int fromX = a.X < b.X ? a.X : b.X;
    int fromY = a.Y < b.Y ? a.Y : b.Y;
    int toX = a.X > b.X ? a.X : b.X;
    int toY = a.Y > b.Y ? a.Y : b.Y;
    fromX = clampIndex(cmToChunkIndex(fromX));
    fromY = clampIndex(cmToChunkIndex(fromY));
    toX = clampIndex(cmToChunkIndex(toX));
    toY = clampIndex(cmToChunkIndex(toY));


    // get all x and y axis and smooth them.
    bezierCurve curve;

    //define this vector out side loop for preventing unesecarry wiping from stack until complete
    //TVector data gets overriden anyway / internal garbage collector
    int scalePrediction = terrainCreator::CHUNKSIZE * map.size(); //one meter gaps obviously.
    TVector<FVector2D> output(scalePrediction); // use only one custom tvector for efficency

    for (int it = 0; it < iterations; it++){

        // all x columns
        int xcount = 0;
        //for (int i = 0; i < map.size(); i++)
        for (int i = fromX; i <= toX; i++)
        {   
            
            for (int innerX = 0; innerX < terrainCreator::CHUNKSIZE; innerX++)
            {
                std::vector<FVector2D> column;
                for (int j = fromY; j <= toY; j++){
                    column.push_back(map.at(i).at(j).getFirstXColumnAnchor(innerX));
                }
                output.clear();
                curve.calculatecurve(column, output, terrainCreator::ONEMETER);

                //trying writing immidately
                applyColumnOrRow(xcount, output, true);
                xcount++;
            }
        }
        



        //then all y rows
        int ycount = 0;
        for (int cY = fromY; cY <= toY; cY++){
            for (int innerY = 0; innerY < terrainCreator::CHUNKSIZE; innerY++)
            {
                std::vector<FVector2D> row;
                //über ganz x laufen und einsammeln
                for (int cX = fromX; cX <= toX; cX++){
                    row.push_back(map.at(cX).at(cY).getFirstYRowAnchor(innerY));
                }

                output.clear();
                curve.calculatecurve(row, output, terrainCreator::ONEMETER);
                
                
                applyColumnOrRow(ycount, output, false);

                ycount++;
            }
        }


    }

}






void terrainCreator::applyColumnOrRow(
    int index, 
    TVector<FVector2D> &data,
    bool isColumn
){
    float newHeight = 0;
    int i_InMeter = index;
    int i_InCm = i_InMeter * terrainCreator::ONEMETER;

    int i_InChunk = cmToChunkIndex(i_InCm); //x chunk index in map
    if(i_InChunk > map.size() - 1){
        return; //issue
    }

    int other_InChunk = 0;

    FVector current;
    FVector prev;

    FVector current_build_vectordebug;

    // convert y as cm to chunk
    for (int i = 0; i < data.size(); i++){

        FVector2D now = data.at(i);
        newHeight = now.Y;
        float other_InCm = std::floor(now.X); //test
        other_InChunk = cmToChunkIndex(other_InCm);
        
        if(verifyIndex(other_InChunk)){

            terrainCreator::chunk *c = nullptr;
            int i_InnerIndex = 0;
            int other_InnerIndex = 0;
            if (isColumn){
                c = &map.at(i_InChunk).at(other_InChunk);
                i_InnerIndex = cmToInnerChunkIndex(i_InCm);
                other_InnerIndex = cmToInnerChunkIndex(other_InCm);

                current = FVector(i_InCm, other_InCm, newHeight);

                current_build_vectordebug = FVector(
                    i_InnerIndex * terrainCreator::ONEMETER + i_InChunk * terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER,
                    other_InnerIndex * terrainCreator::ONEMETER + other_InChunk * terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER,
                    newHeight
                );
            }
            else{
                c = &map.at(other_InChunk).at(i_InChunk);
                i_InnerIndex = cmToInnerChunkIndex(other_InCm);
                other_InnerIndex = cmToInnerChunkIndex(i_InCm);

                current = FVector(other_InCm, i_InCm, newHeight);

                //the same because the coords are flipped above 
                current_build_vectordebug = FVector(
                    i_InnerIndex * terrainCreator::ONEMETER + i_InChunk * terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER,
                    other_InnerIndex * terrainCreator::ONEMETER + other_InChunk * terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER,
                    newHeight
                );
            }

            


            if(c != nullptr){
                c->applyIndivualVertexIndexBased(
                    i_InnerIndex,
                    other_InnerIndex,
                    newHeight,
                    //true,
                    isColumn
                );
                bool draw = false;
                if(draw){
                    DebugHelper::showLineBetween(worldPointer, prev, current, FColor::Orange);
                    DebugHelper::showLineBetween(worldPointer, current, current + FVector(0,0,-100), FColor::Red);
                    
                    //HIER WIRD DER FEHLER SICHTBAR (wenn er auftritt, was er nicht mehr tut.)
                    DebugHelper::showLineBetween(
                        worldPointer,
                        current + FVector(-10, 0, -100),
                        current_build_vectordebug + FVector(-50, 0, -100),
                        FColor::Cyan
                    );

                    prev = current;
                }
            }
        
        }
    }




}











/**
 * 
 * --- EMBED ROOMS ---
 * 
 */

/// @brief will set the are to a certain height provided in the location vector, which should be 
/// the bottom left corner of the rooms created, will smooth the terrain data afterwards
/// @param location location and Z offset to set
/// @param sizeMetersX size in meters X
/// @param sizeMetersY size in meters Y
void terrainCreator::setFlatArea(FVector &location, int sizeMetersX, int sizeMetersY){
    //convert to meters ue5 world scale just as usual
    sizeMetersX *= 100;
    sizeMetersY *= 100;

    //get map bounds with little upscaling
    int fromX = clampIndex(cmToChunkIndex(location.X) - 1);
    int fromY = clampIndex(cmToChunkIndex(location.Y) - 1);
    int toX = clampIndex(fromX + cmToChunkIndex(sizeMetersX) + 1);
    int toY = clampIndex(fromY + cmToChunkIndex(sizeMetersY) + 1);

    int heightToSet = location.Z;

    //iterate over map and smooth
    for (int i = fromX; i <= toX; i++){
        for (int j = fromY; j <= toY; j++){
            terrainCreator::chunk *c = &map.at(i).at(j);
            if(c != nullptr){
                c->setheightForAll(heightToSet);
                c->setTreesBlocked(true);
            }
        }
    }

    //finally also smooth the map
    FVector buffer = FVector(terrainCreator::CHUNKSIZE, terrainCreator::CHUNKSIZE, 0);
    FVector a = location - buffer;
    FVector b(
        sizeMetersX, 
        sizeMetersY,
        0
    );
    b += (a + buffer * 2);

    int iterations = 2;
    smooth3dMap(a, b, iterations); // disabled for debugging
}

/**
 * 
 * --- SCALE CONVERSION SECTION ---
 * 
 */


int terrainCreator::cmToMeter(int a){
    float af = a;
    float result = a / (terrainCreator::ONEMETER * 1.0f);
    // return a / terrainCreator::ONEMETER;
    return std::round(result); //works better!!
}
int terrainCreator::meterToInnerChunkIndex(int a){
    return a % terrainCreator::CHUNKSIZE;
}
/// @brief creates the inner chunk index from cm
/// @param a cm
/// @return index
int terrainCreator::cmToInnerChunkIndex(int a){
    a = cmToMeter(a);
    return meterToInnerChunkIndex(a);
}

/// @brief creates the outer chunk index inside the map for a cm value
/// @param a cm in
/// @return chunk index outer
int terrainCreator::cmToChunkIndex(int a){
    //int cmForOneChunk = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;
    int aToMeter = cmToMeter(a);
    int aToChunk = aToMeter / terrainCreator::CHUNKSIZE;
    return aToChunk;
}

/// @brief checks if the index is within the map bounds
/// @param a index
/// @return true false map bounds kept
bool terrainCreator::verifyIndex(int a){
    return a >= 0 && a < map.size();
}


/// @brief validates the index on either x or y axis
/// @return clamped value valid to acces the 2D vector map
int terrainCreator::clampIndex(int a){
    if(a < 0){
        a = 0;
    }
    if(a >= map.size()){
        a = map.size() - 1;
    }
    return a;
}







/**
 * 
 * plotting
 *
 */


/// @brief debug plotting
/// @param world 
void terrainCreator::plotAllChunks(UWorld * world){
    if (world != nullptr && (terrainCreator::PLOTTING_ENABLED))
    {

        for (int i = 0; i < map.size(); i++){
            for (int j = 0; j < map.at(i).size(); j++){
                map.at(i).at(j).plot(world);
            }
        }
    }
}

/**
 * --- VIRTUAL RAYCAST ---
 * DEBUG NEEDED, ENTETIES YEET IN THE AIR
 */

/// @brief instead of raycasting the z height can be got from the generated mesh data
/// @param position position to find (only x y important)
/// @return return z for the x y position
float terrainCreator::getHeightFor(FVector &position){

    //spannt automatisch das viereck um den vertex weil die position nach unten geclamped wird
    //mit modulo
    //so die idee 
    TArray<FVector> positionIndices = {
        FVector(position.X, position.Y, 0.0f),
        FVector(position.X + ONEMETER, position.Y, 0.0f),
        FVector(position.X, position.Y + ONEMETER, 0.0f),
        FVector(position.X + ONEMETER, position.Y + ONEMETER, 0.0f)
    };

    // find heights for extraction data
    TArray<HeightExtractionData> heights;
    TArray<terrainCreator::chunk *> chunks = chunksAt(positionIndices);
    for (int i = 0; i < chunks.Num(); i++){
        terrainCreator::chunk *ptr = chunks[i];
        FVector &posCurrent = positionIndices[i];
        if (ptr)
        {
            //HeightExtractionData(vertex targeted, target pos for height extraction per weight)
            HeightExtractionData container(posCurrent, position);
            ptr->getHeightWith(container);
            heights.Add(container);
        }
    }

    float outheight = HeightExtractionData::findHeight(heights, ONEMETER);
    return outheight;

}



/// @brief returns the count of the inner chunks in total
/// @return count
int terrainCreator::chunkNum(){
    //is used from the entity manager to pass the mesh actors
    if(map.size() > 0){
        return map.size() * map.at(0).size();
    }
    return 0;
}



/**
 * ---- APPLY DATA TO MESH ACTORS ----
 */


/// @brief will create all meshes and populate the data
/// will not hold on to the reference when goes out of scope, only applies the data
/// where it can and doesnt go out of bounds
void terrainCreator::applyTerrainDataToMeshActors(){

    int x = 0;
    int y = 0;
    //inclusive index limit
    int xLimit = map.size();
    int yLimit = map.size();

    applyTerrainDataToMeshActors(x, xLimit, y, yLimit);

}

void terrainCreator::applyTerrainDataToMeshActors(
    int lowerX,
    int xLimit,
    int lowerY,
    int yLimit
){
    int x = lowerX;
    int y = lowerY;
    int actorIndex = 0;
    int limit = (yLimit - lowerY) * (xLimit - lowerX);
    while (actorIndex < limit)
    {
        actorIndex++;
        createChunkAtIfNotCreatedYet(x, y); //verifies the index automatically!
        x++;
        //top corner reached, return
        if(y >= yLimit && x >= xLimit){
            return;
        }
        //next row
        if(x >= xLimit){
            x = 0;
            y++;
        }
    }
}




void terrainCreator::createChunkAtIfNotCreatedYet(int x, int y){

    terrainCreator::chunk *currentChunk = chunkAt(x,y);
    if(
        (currentChunk != nullptr) && 
        (currentChunk->wasAlreadyCreated() == false)
    ){
        currentChunk->setWasCreatedTrue();

        int xLimit = map.size();
        int yLimit = map.size();

        AcustomMeshActor *currentActor = getNewMeshActor();
        /*
        deprecated! 
        EntityManager *entityManagerPointer = worldLevel::entityManager();
        if(entityManagerPointer != nullptr){
            FVector location(0, 0, 0);
            currentActor = entityManagerPointer->spawnAcustomMeshActor(worldPointer, location);
        }*/
        if(currentActor == nullptr){
            return;
        }

        

        // apply position
        FVector newPos = currentChunk->positionPivotBottomLeft();
        currentActor->SetActorLocation(newPos);

        //apply data
        //readAndMerge (connect to next in map)
        terrainCreator::chunk *top = nullptr;
        terrainCreator::chunk *right = nullptr;
        terrainCreator::chunk *topright = nullptr;

        
        if(y + 1 < yLimit){
            top = &map.at(x).at(y+1);
        }
        if(x + 1 < xLimit){
            right = &map.at(x+1).at(y);
        }
        if(x + 1 < xLimit && y + 1 < yLimit){
            topright = &map.at(x+1).at(y+1);
        }

        TerrainChunkSetup package = currentChunk->makeSetupPackage(top, right, topright);
        ETerrainType terrainType = package.getTerrainType();
        currentActor->createTerrainFrom2DMap(package);



        if(terrainType == ETerrainType::EOcean){
            newPos.Z = HEIGHT_MAX_OCEAN * 0.8f;
            createWaterPaneAt(newPos);
        }

        DebugHelper::showScreenMessage("CREATED NEW CHUNK", FColor::Purple);
    }
}


void terrainCreator::createWaterPaneAt(FVector &location){
    if(worldPointer != nullptr){
        int scaleCm = CHUNKSIZE * ONEMETER;

        AcustomWaterActor::createWaterPane(
            worldPointer,
            location,
            scaleCm
        );
    }
}














/**
 * 
 * 
 * --- create height maps ---
 * 
 * 
 */


/// @brief will create a random height map chunk wide, then to be smoothed
void terrainCreator::createRandomHeightMapChunkWide(int layers){

    for (int i = 0; i < std::abs(layers); i++){
        terrainHillSetup newHill = createRandomHillData();
        applyHillData(newHill);
    }
}


terrainHillSetup terrainCreator::createRandomHillData(){
    int scaleX = FVectorUtil::randomNumber(MINCHUNK_HILL, map.size()); //random hardcoded for now.
    int scaleY = FVectorUtil::randomNumber(MINCHUNK_HILL, map.size());
    return createRandomHillData(scaleX, scaleY);
}

terrainHillSetup terrainCreator::createRandomHillData(
    int sizeX, int sizeY
){
    sizeX = std::abs(sizeX);
    sizeY = std::abs(sizeY);
    if(sizeX <= 0){
        sizeX = 1;
    }
    if(sizeY <= 0){
        sizeY = 1;
    }


    int startX = clampIndex(FVectorUtil::randomNumber(1, map.size() - sizeX));
    int startY = clampIndex(FVectorUtil::randomNumber(1, map.size() - sizeY));
    int heightMin = terrainCreator::ONEMETER / 2.0f;
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

void terrainCreator::applyHillData(std::vector<terrainHillSetup> &hillDataVec){
    for (int i = 0; i < hillDataVec.size(); i++){
        applyHillData(hillDataVec[i]);
    }
}


/// @brief will enheight the map based on the passed hilldata in size X, size Y and height add
/// @param hillData 
void terrainCreator::applyHillData(terrainHillSetup &hillData){
    for (int i = clampIndex(hillData.xPosCopy()); i < clampIndex(hillData.xTargetCopy()); i++){
        for (int j = clampIndex(hillData.yPosCopy()); j < clampIndex(hillData.yTargetCopy()); j++){
            if(verifyIndex(i) && verifyIndex(j)){
                map.at(i).at(j).addheightForAll(hillData.getHeightIfSetOrRandomHeight());
            }
        }
    }
}




void terrainCreator::flattenChunksForHillData(std::vector<terrainHillSetup> &hillDataVec){
    for (int i = 0; i < hillDataVec.size(); i++){
        flattenChunksForHillData(hillDataVec[i]);
    }
}

///@brief clamps an area to a max height defined by the passed hilldata object
void terrainCreator::flattenChunksForHillData(terrainHillSetup &hillData){
    for (int i = clampIndex(hillData.xPosCopy()); i < clampIndex(hillData.xTargetCopy()); i++){
        for (int j = clampIndex(hillData.yPosCopy()); j < clampIndex(hillData.yTargetCopy()); j++){
            if(verifyIndex(i) && verifyIndex(j)){
                //map.at(i).at(j).clampheightForAllUpperLimit(hillData.getForcedSetHeight());
                //map.at(i).at(j).clampheightForAllUpperLimitByOwnAverageHeight();
                map.at(i).at(j).setheightForAllToAverage();

                //disable trees for rooms
                map.at(i).at(j).setTreesBlocked(true);
            }
        }
    }
}







/// @brief randomizes terrain types by enclosing bezier curves
/// @param world 
void terrainCreator::randomizeTerrainTypes(UWorld *world){
    
    int sizeOfShape = 10; //Chunks
    int step = 1;
    FVectorShape shape;

    int shapeCount = map.size();

    std::vector<ETerrainType> terraintypesVector = createRandomTerrainTypes(shapeCount);

    for (int i = 0; i < shapeCount; i++){

        shape.createRandomNewSmoothedShapeClamped(sizeOfShape, step);
        shape.floorAllCoordinateValues(); //macht es quasi eckig

        //DEBUG
        
        if(false){
            std::vector<FVector> vertecies = shape.vectorCopy();
            MMatrix scaleMat;
            scaleMat.scale(100, 100, 1);
            MMatrix rot;
            rot.pitchRadAdd(MMatrix::degToRadian(90.0f));
            MMatrix translate;
            translate.setTranslation(0, 0, 300);

            MMatrix result = rot * scaleMat;
            result = translate * result;

            for (int j = 0; j < vertecies.size(); j++){
                vertecies[j] = result * vertecies[j];
            }
            DebugHelper::showLine(world, vertecies, FColor::Blue);
        }
        
        //DEBUG END



        //random offset into map
        MMatrix moveMatrix;
        moveMatrix.setTranslation(
            FVectorUtil::randomNumber(0, map.size() - sizeOfShape),
            FVectorUtil::randomNumber(0, map.size() - sizeOfShape),
            0
        );
        if(false)
            shape.moveVerteciesWith(moveMatrix); //debug remove

        

        shape.sortVerteciesOnXAxis();
        std::vector<FVector> vertecies = shape.vectorCopy();
        
        if(vertecies.size() > 0){
            
            FVector &chunkAt = vertecies[0];
            int x = clampIndex(chunkAt.X);
            int y = clampIndex(chunkAt.Y);

            for (int vertex = 1; vertex < vertecies.size(); vertex++){
                FVector &prevVertex = vertecies[vertex - 1];
                FVector &currentVertex = vertecies[vertex];

                applyTerrainTypeBetween(prevVertex, currentVertex, terraintypesVector[i]);

                //DebugHelper::logMessage("terrainDebug apply terrain ");
            }
        }

    }
}

///@brief applies a terrain type betweem two coordinates of chunks, where X component
/// is SAME! 
void terrainCreator::applyTerrainTypeBetween(FVector &a, FVector &b, ETerrainType typeIn){
    if(a.X == b.X){
        int xIndex = clampIndex(a.X);

        FVector &smaller = a.Y < b.Y ? a : b;
        FVector &bigger = a.Y > b.Y ? a : b;

        for (int i = smaller.Y; i <= bigger.Y; i++){
            int yIndex = clampIndex(i);

            terrainCreator::chunk *currentChunk = chunkAt(xIndex, yIndex);
            if(currentChunk != nullptr){
                currentChunk->updateTerraintype(typeIn);
            }
        }
    }
}

///@brief creates a random terrain vector in passed count
///@param count of terrain types
std::vector<ETerrainType> terrainCreator::createRandomTerrainTypes(int count){
    std::vector<ETerrainType> outterrain;
    ETerrainType prev = ETerrainType::ETropical;
    for (int i = 0; i < count; i++){
        outterrain.push_back(selectTerrainTypeExcluding(prev));
        prev = outterrain.back();
    }
    return outterrain;
}

///@brief selects a terrain type excluding a target terrain
ETerrainType terrainCreator::selectTerrainTypeExcluding(ETerrainType typeToExclude){
    std::vector<ETerrainType> vector = //AcustomMeshActorBase::terrainVector();
    {
        ETerrainType::EDesert,
        ETerrainType::EDesertForest,
        ETerrainType::ETropical
        //,ETerrainType::ESnowHill
    };

    ETerrainType terraintypeRandom = ETerrainType::ETropical;
    int randomIndex = FVectorUtil::randomNumber(0, vector.size());
    randomIndex %= vector.size();
    terraintypeRandom = vector[randomIndex];
    if(terraintypeRandom == typeToExclude){
        randomIndex = (randomIndex + 1) % vector.size();
        terraintypeRandom = vector[randomIndex];
    }

    return terraintypeRandom;
}

/// @brief returns a chunk by pointer or nullptr if the index was invalid
/// @param x 
/// @param y 
/// @return 
terrainCreator::chunk *terrainCreator::chunkAt(int x, int y){
    if(verifyIndex(x) && verifyIndex(y)){
        return &map[x][y];
    }
    return nullptr;
}

TArray<terrainCreator::chunk *> terrainCreator::chunksAt(
    TArray<FVector> &positionsWorld
){
    TArray<terrainCreator::chunk *> outputArray;
    for (int i = 0; i < positionsWorld.Num(); i++)
    {
        FVector &current = positionsWorld[i];
        terrainCreator::chunk *ptr = chunkAt(
            cmToChunkIndex(current.X),
            cmToChunkIndex(current.Y)
        );
        if(ptr != nullptr){
            outputArray.Add(ptr);
        }
    }
    return outputArray;
}

/// @brief applies the ESnowhill terrain type to chunks matching the minheight requirement
void terrainCreator::applySpecialTerrainTypesByHeight(){
    for (int i = 0; i < map.size(); i++){
        for (int j = 0; j < map[i].size(); j++){
            terrainCreator::chunk *currentChunkPointer = chunkAt(i, j);
            if(currentChunkPointer != nullptr){
                currentChunkPointer->updateTerrainTypeBySpecialHeights();
            }
        }
    }
}


/**
 * 
 * 
 * ---- player tick creation ----
 * 
 * 
 */

///@brief will create surrounding chunks if not created yet
void terrainCreator::Tick(FVector &playerLocation){
    //player to chunkindex
    int x = cmToChunkIndex(playerLocation.X);
    int y = cmToChunkIndex(playerLocation.Y);

    FVector location(x, y, 0);
    //DebugHelper::showScreenMessage("terrain tick chunk", location, FColor::Green);

    //25 mesh actors

    int half = CHUNKSTOCREATEATONCE / 2;
    half = 2;

    //debug
    half = map.size();

    applyTerrainDataToMeshActors(
        x - CHUNKSTOCREATEATONCE,
        x + CHUNKSTOCREATEATONCE,
        y - CHUNKSTOCREATEATONCE,
        y + CHUNKSTOCREATEATONCE
    );
}

/**
 * 
 * --- Example method for creating the terrain ---
 * 
 */
void terrainCreator::debugCreateTerrain(UWorld *world){
    //createTerrainAndSpawnMeshActors(world, 200);

    //new with player tick needed
    //createTerrainAndCreateBuildings(world, 200);
    debugCreateTerrain(world, 200);
}

void terrainCreator::debugCreateTerrain(UWorld *world, int meters){
    //createTerrainAndSpawnMeshActors(world, 200); //old

    meters = std::abs(meters);
    meters = std::max(meters, CHUNKSIZE * 2);
    createTerrainAndCreateBuildings(world, meters); //new
}

/// @brief creates a terrain and brand new mesh actors without using the entity manager
/// @param world world to spawn in, must not be nullptr
/// @param meters meters xy of terrain
void terrainCreator::createTerrainAndSpawnMeshActors(
    UWorld *world, int meters
){
    meters = std::abs(meters);
    if(meters < 100){
        meters = 100;
    }

    if(world != nullptr){
        FVector location(0, 0, 0);
        createTerrain(world, meters);

        int numberCreated = chunkNum();

        randomizeTerrainTypes(world);
        applySpecialTerrainTypesByHeight();
        applyTerrainDataToMeshActors();
    }
}



/**
 * create outposts section / buildings
 */

///@brief creates the buildings and the terrain, but only will spawn terrain if the player is
///near enough on tick
void terrainCreator::createTerrainAndCreateBuildings(
    UWorld *world, int meters
){
    int chunkRange = meters / CHUNKSIZE;

    DebugHelper::logMessage("debugterrain METERS ", meters);
    DebugHelper::logMessage("debugterrain CHUNKS ", chunkRange);

    int count = 3;
    int minsizeChunks = 1;
    int maxsizeChunks = 3;

    std::vector<terrainHillSetup> predefinedHillDataVecFlatArea;
    createFlatAreas(count, minsizeChunks, maxsizeChunks, chunkRange, predefinedHillDataVecFlatArea);
    createTerrain(world, meters, predefinedHillDataVecFlatArea);

    DebugHelper::logMessage("debugterrain hill data size ", predefinedHillDataVecFlatArea.size());

    randomizeTerrainTypes(world);
    applySpecialTerrainTypesByHeight();
    
    //recursion issue ? 
    //use this data to create the buildings
    //predefinedHillDataVecFlatArea
    //based also on terrain type!
    std::vector<terrainCreator::chunk *> chunksFound;
    findChunksEnclosedBy(
        predefinedHillDataVecFlatArea,
        chunksFound
    );
    DebugHelper::logMessage("debugterrain chunks found", chunksFound.size());
    
    for (int i = 0; i < chunksFound.size(); i++){
        terrainCreator::chunk *currentPointer = chunksFound[i];
        if(currentPointer != nullptr){
            FVector posPivot = currentPointer->positionPivotBottomLeft();
            posPivot.Z = currentPointer->maxHeight();

            //create building there.
            int sizeMaxMeters = CHUNKSIZE;
            sizeMaxMeters -= 3;
            AroomProcedural::generate(world, sizeMaxMeters, sizeMaxMeters, posPivot); //in size is METERS

        }
    }

    //spawn all - managed via tick(?).
    //applyTerrainDataToMeshActors();

    markCreateOutpostsAt(predefinedHillDataVecFlatArea);
    createRoads(world);
}

/// @brief creates a output vector of terrainHillsetup in chunk index boundign boxes
/// @param count 
/// @param minsizeChunks 
/// @param maxsizeChunks 
/// @param chunkRange 
/// @param output 
void terrainCreator::createFlatAreas(
    int count, 
    int minsizeChunks, 
    int maxsizeChunks,
    int chunkRange,
    std::vector<terrainHillSetup> &output
){
    for (int i = 0; i < count; i++){
        createFlatArea(minsizeChunks, maxsizeChunks, chunkRange, output);
    }
}


/// @brief creates a terrainHillsetup in chunk index boundign box space
/// @param minsizeChunks 
/// @param maxsizeChunks 
/// @param chunkRange 
/// @param output 
void terrainCreator::createFlatArea(
    int minsizeChunks, 
    int maxsizeChunks,
    int chunkRange,
    std::vector<terrainHillSetup> &output
){
    int scaleX = 0;
    int scaleY = 0;
    int startX = 0;
    int startY = 0;
    int i = 1000;
    while(i > 0){

        scaleX = std::abs(FVectorUtil::randomNumber(minsizeChunks, maxsizeChunks));
        scaleY = std::abs(FVectorUtil::randomNumber(minsizeChunks, maxsizeChunks));

        int min = 3;
        startX = std::abs(FVectorUtil::randomNumber(min, chunkRange - scaleX));
        startY = std::abs(FVectorUtil::randomNumber(min, chunkRange - scaleY));

        bool overlapFound = false;
        for (int t = 0; t < output.size(); t++)
        {
            if(output[t].doesOverlapArea(startX, startY, scaleX, scaleY)){
                overlapFound = true;
                t = output.size();
            }
        }
        if(!overlapFound){
            break;
        }

        i--;
    }

    int heightMin = 0;
    int heightMax = 0;

    terrainHillSetup newHillSetup(
        startX,
        startY,
        scaleX,
        scaleY,
        heightMin,
        heightMax
    );
    newHillSetup.forceSetHeight(100); //debug

    //terrainHillSetup newHillSetup = createRandomHillData(scaleX, scaleY);

    

    output.push_back(newHillSetup);
}





///@brief helps to find all chunks enclosed by the hill setup passed
//use only temporary, pointers might get invalid if you change the map!
void terrainCreator::findChunksEnclosedBy(
    std::vector<terrainHillSetup> &hills,
    std::vector<terrainCreator::chunk *> &output
){
    std::set<terrainCreator::chunk *> outset;
    for (int i = 0; i < hills.size(); i++){
        findChunksEnclosedBy(hills[i], outset);
    }

    for(auto &current : outset){
        output.push_back(current);
    }
}


///@brief helps to find all chunks enclosed by the hill setup passed
//use only temporary, pointers might get invalid if you change the map!
void terrainCreator::findChunksEnclosedBy(
    terrainHillSetup &hillData,
    std::set<terrainCreator::chunk *> &output
){
    for (int i = clampIndex(hillData.xPosCopy()); i < clampIndex(hillData.xTargetCopy()); i++){
        for (int j = clampIndex(hillData.yPosCopy()); j < clampIndex(hillData.yTargetCopy()); j++){
            if(verifyIndex(i) && verifyIndex(j)){

                //if not contained yet, add
                terrainCreator::chunk *ptr = chunkAt(i,j);
                if(ptr != nullptr){
                    output.insert(ptr);
                }
                
            }
        }
    }
}




void terrainCreator::markCreateOutpostsAt(
    std::vector<terrainHillSetup> &predefinedHillDataVecFlatArea
){
    if(worldPointer != nullptr){
        float scaleUpFactor = CHUNKSIZE * ONEMETER;

        std::vector<FVector> worldCoordinates;
        for (int i = 0; i < predefinedHillDataVecFlatArea.size(); i++){
            terrainHillSetup &current = predefinedHillDataVecFlatArea[i];

            FVector center = current.center();
            int x = center.X;
            int y = center.Y;

            if(verifyIndex(x) && verifyIndex(y)){

                //if not contained yet, add
                terrainCreator::chunk *ptr = chunkAt(x,y);
                if(ptr != nullptr){
                    ptr->markCreateOutpostTrue();
                }
            }
        }
    }
}




AcustomMeshActor *terrainCreator::getNewMeshActor(){
    AcustomMeshActor *meshActor = nullptr;
    EntityManagerBase *entityManagerPointer = EntityManagerBase::instanceBase();
    if(entityManagerPointer){
        meshActor = entityManagerPointer->requestByEnum<AcustomMeshActor>(
            ETrackedActors::EMeshActor,
            worldPointer
        );
    }
    return meshActor;
}

/**
 * ROAD SECTION
 */
void terrainCreator::createRoads(UWorld *world){
    
    if(world){
        AcustomMeshActor *currentActor = getNewMeshActor();
        if (currentActor == nullptr)
        {
            return;
        }
        currentActor->disableDistanceListening();
        MeshData &meshdata = currentActor->findMeshDataReference(
            materialEnum::stoneMaterial,
            true //has raycast
        );
        createRoads(meshdata, 2);
        currentActor->ReloadMeshAndApplyAllMaterials();
    }

}

void terrainCreator::createRoads(MeshData &meshdata, int count){
    //debug
    //return;

    for(int i = 0; i < count; i++){
        createRoad(meshdata);
    }
}


void terrainCreator::createRoad(MeshData &meshdata){
    int chunks = map.size();
    int scalePerChunk = CHUNKSIZE * ONEMETER;
    int limitall = scalePerChunk * chunks;
	
    //eine strasse erstreckt sich immer über die ganze karte z.b.


    bezierCurve curve;
    TVector<FVector2D> output;
    float _einheitsValue = ONEMETER;

    if(true){
        /*
        FVector2D &startingPoint,
        TVector<FVector2D> &output,
        float _einheitsValue,
        float distanceBetweenAnchorsOnXAxisMin,
        float distanceBetweenAnchorsOnXAxisMax,
        float distanceBetweenAnchorsYRange,
        float max_xy_coordinate
        */

        FVector2D startingPoint;
        float distanceBetweenAnchorsOnXAxisMin = scalePerChunk * 1.0f;
        float distanceBetweenAnchorsOnXAxisMax = scalePerChunk * 3.0f;
        float distanceBetweenAnchorsYRange = scalePerChunk;
        float max_xy_coordinate = limitall;

        curve.createNewRandomCurve(
            startingPoint,
            output,
            _einheitsValue,
            distanceBetweenAnchorsOnXAxisMin,
            distanceBetweenAnchorsOnXAxisMax,
            distanceBetweenAnchorsYRange,
            max_xy_coordinate
        );
    }else{
        /*
        FVector2D &startingPoint,
        TVector<FVector2D> &output,
        float _einheitsValue,
        float distanceMin,
        float distanceMax,
        float angleTurnMinAbs,
        float angleTurnMaxAbs,
        float max_xy_coordinate
        */
        FVector2D start(
            0, //goes along x.
            FVectorUtil::randomNumber(0, limitall)
        );
        float distanceBetweenAnchorsOnXAxisMin = scalePerChunk * 1.0f;
        float distanceBetweenAnchorsOnXAxisMax = scalePerChunk * 3.0f;
        float angleTurnMin = 10.0f;
        float angleTurnMax = 30.0f;
        float max_xy_coordinate = limitall;
        curve.createNewRandomCurve(
            start,
            output,
            _einheitsValue,
            distanceBetweenAnchorsOnXAxisMin,
            distanceBetweenAnchorsOnXAxisMax,
            angleTurnMin,
            angleTurnMax,
            max_xy_coordinate
        );


        //draw small version
        for (int i = 1; i < output.size(); i++){
            FVector2D prev = output[i - 1] / (terrainCreator::ONEMETER * terrainCreator::CHUNKSIZE);
            FVector2D current = output[i] / (terrainCreator::ONEMETER * terrainCreator::CHUNKSIZE);
            FVector prev3D(prev.X, 0.0f, prev.Y);
            FVector current3D(current.X, 0.0f, current.Y);

            DebugHelper::showLineBetween(
                worldPointer,
                prev3D,
                current3D,
                FColor::Red
            );
        }
    }

    float roadWidth = ONEMETER * 5.0f;
    processRoad(output, roadWidth, meshdata, _einheitsValue);

    //assign data to mesh actor when done.

}

void terrainCreator::processRoad(
    TVector<FVector2D> &curve,
    float roadWidth,
    MeshData &outmeshData,
    float _einheitsValue
){
    TArray<FVector> line1;
    TArray<FVector> line2;
    line1.SetNum(curve.size());
    line2.SetNum(curve.size());


    roadWidth = std::abs(roadWidth);
    float halfWidht = roadWidth / 2.0f;
    for(int i = 1; i < curve.size(); i++){
        FVector2D &prev = curve[i-1];
        FVector2D &current = curve[i];

        //an normla extenden und beide seiten vertecies suchen, higher one
        FVector2D AB = current - prev; //AB = B - A;
        FVector2D normal(AB.Y, -AB.X); 
        normal = normal.GetSafeNormal();

        FVector2D side0_2d = prev + normal * halfWidht;
        FVector2D side1_2d = prev + -1.0f * normal * halfWidht;

        float side0Heigth = getHeightFor(side0_2d);
        float side1Heigth = getHeightFor(side1_2d);
        float maxHeight = std::max(side0Heigth, side1Heigth);

        FVector side0 = make3D(side0_2d, maxHeight);
        FVector side1 = make3D(side1_2d, maxHeight);

        line1[i] = side0;
        line2[i] = side1;

    }


    //make road look less clunky
    bezierCurve bezierCurveMaker;
    int skipIndicesForSmooth = 10;
    bezierCurveMaker.afterSmoothHeight(
        line1, //TArray<FVector> &curve,
        _einheitsValue,
        skipIndicesForSmooth//int anchorSkipPerStep
    );
    bezierCurveMaker.afterSmoothHeight(
        line2, 
        _einheitsValue,
        skipIndicesForSmooth
    );



    //generate buffer:
    /*
    1->2
    |  |
    0<-3
    */
    outmeshData.appendParalellLinesClosedAsQuads(line1, line2);

    lockQuadsFromParalellArrayLines(line1, line2);


}

FVector terrainCreator::make3D(FVector2D &pos, float height){
    return FVector(
        pos.X,
        pos.Y,
        height
    );
}

float terrainCreator::getHeightFor(FVector2D &pos){
    FVector pos3d(pos.X, pos.Y, 0.0f);
    return getHeightFor(pos3d);
    /*
    int x = cmToChunkIndex(pos.X);
    int y = cmToChunkIndex(pos.Y);
    terrainCreator::chunk *ptr = chunkAt(x,y);
    if(ptr != nullptr){
        FVector pos3d(pos.X, pos.Y, 0.0f);
        int height = ptr->getHeightFor(pos3d);
        return height;
    }
    return 0.0f;*/
}


void terrainCreator::lockQuadsFromParalellArrayLines(
    TArray<FVector> &line0,
    TArray<FVector> &line1
){
    /*
    array aufbau
    
    1   2-1  2

    0   3-0  3... paralelle lines, bilden quads durch adjazente indices
    
    heisst: 0 und 2 bzw 1 und 3 bilden bounding box
    */
    int limit = std::min(line0.Num(), line1.Num());
    for(int i = 1; i < limit; i++){
        FVector &v2 = line1[i];
        FVector &v0 = line0[i-1];

        if(false){
            /**
             * CAUTION: is still. Bugged.
             */

            FVector &v1 = line1[i-1];
            FVector &v3 = line0[i];
            TArray<FVector> positions = {v0, v1, v2, v3};
            TArray<terrainCreator::chunk *> chunksCollected = chunksAt(positions);

            for (int j = 0; j < chunksCollected.Num(); j++){
                terrainCreator::chunk *currentChunk = chunksCollected[j];
                if(currentChunk != nullptr){
                    currentChunk->blockAreaForFoliage(v0, v1, v2, v3);
                }
            }
        }else{
            //old
            //chunk 0
            terrainCreator::chunk *ptr = chunkAt(
                cmToChunkIndex(v0.X),
                cmToChunkIndex(v0.Y)
            );
            if(ptr != nullptr){
                ptr->blockAreaForFoliage(v0,v2);
            }
            
            //chunk 1
            terrainCreator::chunk *ptr1 = chunkAt(
                cmToChunkIndex(v2.X),
                cmToChunkIndex(v2.Y)
            );
            if(ptr1 != nullptr){
                ptr1->blockAreaForFoliage(v0,v2);
            }
        }
        
    }



}
