// Fill out your copyright notice in the Description page of Project Settings.

#include <cmath>
#include "p2/util/FVectorUtil.h"
#include "Algo/Sort.h"  // Include the necessary header
#include "bezierCurve.h"
#include "p2/util/TVector.h"
#include "HAL/PlatformTime.h"
#include <algorithm>
#include "p2/meshgen/foliage/ETerrainType.h"
#include "p2/entities/customIk/MMatrix.h"
#include "p2/meshgen/foliage/helper/FVectorShape.h"
#include "terrainCreator.h"

terrainCreator::terrainCreator()
{
}

terrainCreator::~terrainCreator()
{
    for (int i = 0; i < map.size(); i++){
        map.at(i).clear();
    }
    map.clear();
}

/***
 * 
 * ---- CHUNK METHODS -----
 * 
 */

terrainCreator::chunk::chunk(int xPos, int yPos)
{
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
        for (int j = 0; j < limit; j++){ //y
            FVector pos(
                i * terrainCreator::ONEMETER, 
                j * terrainCreator::ONEMETER, 
                0.0f
            );
            vec.push_back(pos);
        }
        innerMap.push_back(vec);
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

void terrainCreator::chunk::setTreesBlocked(bool b){
    blockTrees = b;
}
bool terrainCreator::chunk::createTrees(){
    return !blockTrees;
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
        //new column to be added
        std::vector<FVector> firstColFromNextRight = right->readFirstXColumn();

        for (int i = 0; i < firstColFromNextRight.size(); i++){
            //override 
            float currentVecHeight = firstColFromNextRight.at(i).Z;
            innerMap.at(innerMap.size() - 1).at(i).Z = currentVecHeight;
        }
    }
    
    //might be wrong!
    if(top != nullptr){
        std::vector<FVector> firstRowFromNextTop = top->readFirstYRow();
        //add row (to each column 1)
        for (int j = 0; j < firstRowFromNextTop.size(); j++){
            if(j < innerMap.size()){
                //override
                FVector current = firstRowFromNextTop.at(j);
                innerMap.at(j).at(innerMap.size() - 1).Z = current.Z;
            }
        }
    }


    if(topRight != nullptr){
        FVector topRightCorner = topRight->readBottomLeftCorner();
        
        //override
        innerMap.at(innerMap.size() - 1).at(innerMap.size() - 1).Z = topRightCorner.Z;
    }

    return innerMap;
}


/**
 * RAYCAST
 */
/// @brief get the height for an specific vertex
/// @param a vertex
/// @return 
int terrainCreator::chunk::getHeightFor(FVector &a){
    if(isInBounds(a)){
        //apply offset because mesh center is at center 
        //int offsetCenter = terrainCreator::ONEMETER * (innerMap.size() / 2); //moved to chunk instantiation
        int xa = convertToInnerIndex(a.X);
        int ya = convertToInnerIndex(a.Y);
        return innerMap.at(xa).at(ya).Z;
    }
    return a.Z;
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
/// @param value in cm!
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

/// @brief will check if a position is within the chunks bounds
/// @param a position to check
/// @return return true if within the chunks index bounds on x and y axis
bool terrainCreator::chunk::isInBounds(FVector &a){
    int total = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;
    int selfX = x * total;
    int selfY = y * total;
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
    sum /= vertexCountAll;
    return sum;
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

/***
 * 
 * ---- TERRAIN METHODS -----
 * 
 */

/// @brief will always create a qudratic terrain
/// @param world world to spawn in / debug draw in
/// @param meters size in meters
void terrainCreator::createTerrain(UWorld *world, int meters){
    worldPointer = world;

    int chunks = floor(meters / terrainCreator::CHUNKSIZE); //to chunks
    //int detail = CHUNKSIZE; // 1 by 1 detail

    //fill map
    for (int i = 0; i < chunks; i++){
        std::vector<terrainCreator::chunk> vec;
        for (int j = 0; j < chunks; j++){
            chunk c(i,j);
            vec.push_back(c);
        }
        map.push_back(vec);
    }


    //random height and smooth
    int layers = 20;
    createRandomHeightMapChunkWide(layers);
    smooth3dMap();

    //plotAllChunks(world);
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
int terrainCreator::getHeightFor(FVector &position){

    int chunkX = cmToChunkIndex(position.X);
    int chunkY = cmToChunkIndex(position.Y);

    if(verifyIndex(chunkX) && verifyIndex(chunkY)){
        return map.at(chunkX).at(chunkY).getHeightFor(position);
    }
    return position.Z;
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

void terrainCreator::applyTerrainDataToMeshActors(std::vector<AcustomMeshActor*> &actors){
    std::vector<AcustomMeshActorBase *> casted;
    for (int i = 0; i < actors.size(); i++){
        AcustomMeshActor *currentPointer = actors.at(i);
        if(currentPointer != nullptr){
            AcustomMeshActorBase *castedPointer = dynamic_cast<AcustomMeshActorBase*>(currentPointer);
            if(castedPointer != nullptr){
                casted.push_back(castedPointer);
            }
        }
    }
    applyTerrainDataToMeshActors(casted);
}

/// @brief will create all meshes and populate the data
/// will not hold on to the reference when goes out of scope, only applies the data
/// where it can and doesnt go out of bounds
void terrainCreator::applyTerrainDataToMeshActors(std::vector<AcustomMeshActorBase*> &actors){


    int actorIndex = 0;
    int x = 0;
    int y = 0;
    //inclusive index limit
    int xLimit = map.size();
    int yLimit = map.size();


    while(actorIndex < actors.size()){
        
        AcustomMeshActorBase *currentActor = actors.at(actorIndex);
        actorIndex++;
        if (currentActor != nullptr)
        {   
            terrainCreator::chunk *currentChunk = &map.at(x).at(y);
            
            // get position
            // apply position
            FVector newPos = currentChunk->position();

            //fix offset to be anchor at bottom left and not center of the mesh,
            //so the coordinate in mesh (0,0) is (0,0) and not (chunksize /2, chunksize/2)
            float offsetCenter = terrainCreator::ONEMETER * (terrainCreator::CHUNKSIZE / 2);
            newPos.X -= offsetCenter;
            newPos.Y -= offsetCenter;
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

            std::vector<std::vector<FVector>> &mapReference = currentChunk->readAndMerge(top, right, topright);

            bool createTrees = currentChunk->createTrees();
            ETerrainType terrainType = currentChunk->getTerrainType();
            currentActor->createTerrainFrom2DMap(mapReference, createTrees, terrainType);

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
    int startX = clampIndex(FVectorUtil::randomNumber(1, map.size() / 2));
    int startY = clampIndex(FVectorUtil::randomNumber(1, map.size() / 2));

    int heightMin = terrainCreator::ONEMETER;
    int heightMax = heightMin * 2;

    return terrainHillSetup(
        startX,
        startY,
        scaleX,
        scaleY,
        heightMin,
        heightMax
    );
}


/// @brief will enheight the map based on the passed hilldata in size X, size Y and height add
/// @param hillData 
void terrainCreator::applyHillData(terrainHillSetup &hillData){
    for (int i = clampIndex(hillData.xPosCopy()); i < clampIndex(hillData.xTargetCopy()); i++){
        for (int j = clampIndex(hillData.yPosCopy()); j < clampIndex(hillData.yTargetCopy()); j++){
            map.at(i).at(j).addheightForAll(hillData.getRandomHeightFromRange());
        }
    }
}






/// @brief randomizes terrain types by enclosing bezier curves
/// @param world 
void terrainCreator::randomizeTerrainTypes(UWorld *world){
    std::vector<ETerrainType> vector //= AcustomMeshActorBase::terrainVector();
        = {
            ETerrainType::EDesert,
            ETerrainType::ETropical
        };

    int sizeOfShape = 10; //Chunks
    int step = 1;
    FVectorShape shape;

    int shapeCount = map.size() / 3;

    for (int i = 0; i < shapeCount; i++){


        ETerrainType terraintypeRandom = ETerrainType::ETropical;
        int randomIndex = FVectorUtil::randomNumber(0, vector.size());
        randomIndex %= vector.size();
        terraintypeRandom = vector[randomIndex];
        
        terraintypeRandom = ETerrainType::EDesert; //DEBUG!

        shape.createRandomNewSmoothedShapeClamped(sizeOfShape, step);
        shape.floorAllCoordinateValues();

        //random offset into map
        MMatrix moveMatrix;
        moveMatrix.setTranslation(
            FVectorUtil::randomNumber(0, map.size() - sizeOfShape),
            FVectorUtil::randomNumber(0, map.size() - sizeOfShape),
            0
        );
        shape.moveVerteciesWith(moveMatrix);

        //DEBUG
        /*
        if(true){
            std::vector<FVector> vertecies = shape.vectorCopy();
            MMatrix m;
            m.scale(-100, -100, 1);
            for (int j = 0; j < vertecies.size(); j++)
            {
                vertecies[j] = m * vertecies[j];
                vertecies[j].Z = 200.0f;
            }
            DebugHelper::showLine(world, vertecies, FColor::Blue);
        }
        */
        //DEBUG END

        shape.sortVerteciesOnXAxis();
        std::vector<FVector> vertecies = shape.vectorCopy();
       
        

        for (int vertex = 1; vertex < vertecies.size(); vertex++){
            FVector &prevVertex = vertecies[vertex - 1];
            FVector &currentVertex = vertecies[vertex];
            applyTerrainTypeBetween(prevVertex, currentVertex, terraintypeRandom);
        }
    }
}

void terrainCreator::applyTerrainTypeBetween(FVector &a, FVector &b, ETerrainType typeIn){
    if(a.X == b.X){
        int xIndex = clampIndex(a.X);

        FVector &smaller = a.Y < b.Y ? a : b;
        FVector &bigger = a.Y > b.Y ? a : b;

        for (int i = smaller.Y; i < bigger.Y; i++){
            int yIndex = clampIndex(i);

            terrainCreator::chunk *currentChunk = chunkAt(xIndex, yIndex);
            if(currentChunk != nullptr){
                currentChunk->updateTerraintype(typeIn);
            }
        }
    }
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


/// @brief applies the ESnowhill terrain type to chunks matching the minheight requirement
void terrainCreator::applySpecialTerrainTypesByHeight(){
    for (int i = 0; i < map.size(); i++){
        for (int j = 0; j < map[i].size(); j++){
            terrainCreator::chunk *currentChunkPointer = chunkAt(i, j);
            if(currentChunkPointer != nullptr){
                float currentHeightAverage = currentChunkPointer->heightAverage();
                if(currentHeightAverage > HEIGH_AVG_SNOWHILL_LOWERBOUND){
                    currentChunkPointer->updateTerraintype(ETerrainType::ESnowHill);
                }
            }
        }
    }
}






/**
 * 
 * --- Example method for creating the terrain ---
 * 
 */
void terrainCreator::debugCreateTerrain(UWorld *world){
    createTerrainAndSpawnMeshActors(world, 200);
}


/// @brief creates a terrain and brand new mesh actors without using the entity manager
/// @param world world to spawn in, must not be nullptr
/// @param meters meters xy of terrain
void terrainCreator::createTerrainAndSpawnMeshActors(UWorld *world, int meters){
    meters = std::abs(meters);
    if(meters < 100){
        meters = 100;
    }

    if(world != nullptr){
        FVector location(0, 0, 0);
        createTerrain(world, meters);

        int numberCreated = chunkNum();
        std::vector<AcustomMeshActor *> meshactors;// = e->requestMeshActors(world, numberCreated);

        
        for (int i = 0; i < numberCreated; i++)
        {
            FRotator rotation;
            FActorSpawnParameters params;
            AcustomMeshActor *SpawnedActor = world->SpawnActor<AcustomMeshActor>(
                AcustomMeshActor::StaticClass(),
                location,
                FRotator::ZeroRotator,
                params
            );
            if(SpawnedActor != nullptr){
                meshactors.push_back(SpawnedActor);
            }
        }

        randomizeTerrainTypes(world);
        applySpecialTerrainTypesByHeight();
        applyTerrainDataToMeshActors(meshactors);
    }
}