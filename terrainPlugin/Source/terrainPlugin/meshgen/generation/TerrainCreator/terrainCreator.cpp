// Fill out your copyright notice in the Description page of Project Settings.
#include "terrainCreator.h"

#include <cmath>
#include "GameCore/util/FVectorUtil.h"
#include "Algo/Sort.h"  // Include the necessary header
#include "terrainPlugin/meshgen/generation/bezierCurve.h"
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

#include "terrainPlugin/meshgen/generation/TerrainCreator/ChunkSetup/TerrainChunkAttributes.h"

#include "terrainConstants.h"



terrainCreator::terrainCreator()
{
}

terrainCreator::~terrainCreator()
{

}





void terrainCreator::createArray(int chunks){
    chunks = std::abs(chunks);

    //clear map
    map.clear();

    //fill map
    map.reserve(chunks);
    for (int i = 0; i < chunks; i++){
        std::vector<chunk> vec;
        vec.reserve(chunks);
        for (int j = 0; j < chunks; j++){
            chunk c(i,j);
            vec.push_back(c);
        }
        map.push_back(vec);
    }
}





void terrainCreator::setupFromChunkMapQuadPart(
    TerrainChunkMap &ref, 
    int x, 
    int y, 
    int numChunksSide
){
    DebugHelper::logMessage("TerrainCreator new setup!");
    TArray<FTerrainChunkAttributes *> attributes = ref.getQuad(x, y, numChunksSide);
    for (int i = 0; i < attributes.Num(); i++){
        FTerrainChunkAttributes *current = attributes[i];
        if(current){
            int posX = current->posX;
            int posY = current->posY;
            int height = current->height;
            bool outpostFlag = current->outpostFlagged;
            bool buildingFlag = current->buildingFlagged;

            chunk *chunkPtr = chunkAt(posX, posY);
            if(chunkPtr){
                chunkPtr->setheightForAll(height);

                if(outpostFlag){
                    chunkPtr->markCreateOutpostTrue();
                }
                if(buildingFlag){
                    chunkPtr->markBuildingCreateTrueAndBlockTrees();
                }
            }
        }
    }
}






/**
 * ----- 3D SMOOTH MAP SECTION -----
 */

/// @brief will smooth out all chunks rows and columns and merge them together to the map
void terrainCreator::smooth3dMap(){
    FVector a(0, 0, 0);
    int max = map.size() * terrainConstants::ONEMETER * terrainConstants::CHUNKSIZE;
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
    int scalePrediction = terrainConstants::CHUNKSIZE * map.size(); //one meter gaps obviously.
    TVector<FVector2D> output(scalePrediction); // use only one custom tvector for efficency

    for (int it = 0; it < iterations; it++){

        // all x columns
        int xcount = 0;
        //for (int i = 0; i < map.size(); i++)
        for (int i = fromX; i <= toX; i++)
        {   
            
            for (int innerX = 0; innerX < terrainConstants::CHUNKSIZE; innerX++)
            {
                std::vector<FVector2D> column;
                for (int j = fromY; j <= toY; j++){
                    column.push_back(map.at(i).at(j).getFirstXColumnAnchor(innerX));
                }
                output.clear();
                curve.calculatecurve(column, output, terrainConstants::ONEMETER);

                //trying writing immidately
                applyColumnOrRow(xcount, output, true);
                xcount++;
            }
        }
        



        //then all y rows
        int ycount = 0;
        for (int cY = fromY; cY <= toY; cY++){
            for (int innerY = 0; innerY < terrainConstants::CHUNKSIZE; innerY++)
            {
                std::vector<FVector2D> row;
                //über ganz x laufen und einsammeln
                for (int cX = fromX; cX <= toX; cX++){
                    row.push_back(map.at(cX).at(cY).getFirstYRowAnchor(innerY));
                }

                output.clear();
                curve.calculatecurve(row, output, terrainConstants::ONEMETER);
                
                
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
    int i_InCm = i_InMeter * terrainConstants::ONEMETER;

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

            chunk *c = nullptr;
            int i_InnerIndex = 0;
            int other_InnerIndex = 0;
            if (isColumn){
                c = &map.at(i_InChunk).at(other_InChunk);
                i_InnerIndex = cmToInnerChunkIndex(i_InCm);
                other_InnerIndex = cmToInnerChunkIndex(other_InCm);

                current = FVector(i_InCm, other_InCm, newHeight);

                current_build_vectordebug = FVector(
                    i_InnerIndex * terrainConstants::ONEMETER + i_InChunk * terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER,
                    other_InnerIndex * terrainConstants::ONEMETER + other_InChunk * terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER,
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
                    i_InnerIndex * terrainConstants::ONEMETER + i_InChunk * terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER,
                    other_InnerIndex * terrainConstants::ONEMETER + other_InChunk * terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER,
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
 * --- SCALE CONVERSION SECTION ---
 * 
 */


int terrainCreator::cmToMeter(int a){
    float af = a;
    float result = a / (terrainConstants::ONEMETER * 1.0f);
    // return a / terrainConstants::ONEMETER;
    return std::round(result); //works better!!
}
int terrainCreator::meterToInnerChunkIndex(int a){
    return a % terrainConstants::CHUNKSIZE;
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
    //int cmForOneChunk = terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER;
    int aToMeter = cmToMeter(a);
    int aToChunk = aToMeter / terrainConstants::CHUNKSIZE;
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

    
    //create pane at world vertecies and perform hittest with FMath.
    TArray<FVector> positionIndices = {
        FVector(position.X, position.Y, 0.0f),
        FVector(position.X + terrainConstants::ONEMETER, position.Y, 0.0f),
        FVector(position.X, position.Y + terrainConstants::ONEMETER, 0.0f)
        //,FVector(position.X + ONEMETER, position.Y + ONEMETER, 0.0f)
    };

    //NEW Collect vertecies
    TArray<FVector> worldVertecies;
    TArray<chunk *> chunks = chunksAt(positionIndices);
    for (int i = 0; i < chunks.Num(); i++){
        FVector newPos;
        chunk *current = chunks[i];
        if(current && i < positionIndices.Num()){
            if(current->NextWorldVertexAt(positionIndices[i], newPos)){
                worldVertecies.Add(newPos); //closest vertex added
            }
        }
    }

    /*
    FPlane(va,vb,vc)

    template<typename FReal>  
    static UE::Math::TVector < FReal > LinePlaneIntersection  
    (  
        const UE::Math::TVector < FReal > & Point1,  
        const UE::Math::TVector < FReal > & Point2,  
        const UE::Math::TPlane < FReal > & Plane  
    )
    */

    //is tested works very well
    if(worldVertecies.Num() == 3){
        FVector start = position + FVector(0, 0, 10000);
        FVector end = position - FVector(0, 0, 10000);
        FPlane plane(
            worldVertecies[0],
            worldVertecies[1],
            worldVertecies[2]
        );
        //DebugHelper::showScreenMessage("plane test", FColor::Orange);
        FVector hit;
        if (FMath::SegmentPlaneIntersection(start, end, plane, hit))
        {
            //DebugHelper::showScreenMessage("plane test hit", FColor::Orange);
            //DebugHelper::showLineBetween(worldPointer, hit, hit + FVector(0, 0, 100), FColor::Red, 0.5f);
            return hit.Z;
        }
    }
    return 0.0f;

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
 * 
 * ---- APPLY DATA TO MESH ACTORS - THROUGH CHUNK PARSER MAP ----
 * 
 */
void terrainCreator::createTerrainAndSetupChunkParserMap(
    TerrainChunkMap &heightMap,
    ChunkParserMap &mapToFillDataTo
){

    createArray(heightMap.countPerAxis());
    setupFromChunkMapQuadPart(
        heightMap,
        0,
        0,
        heightMap.countPerAxis()
    );
    smooth3dMap();
    randomizeTerrainTypes();
    applySpecialTerrainTypesByHeight();

    //manual flat areas are missing here. - must be added in terrain chunk map

    //fill
    applyTerrainDataIntoChunkParserMapCache(mapToFillDataTo);
}

void terrainCreator::applyTerrainDataIntoChunkParserMapCache(
    ChunkParserMap &mapToFillDataTo
){
    for (int i = 0; i < map.size(); i++){
        for (int j = 0; j < map[i].size(); j++){
            applyTerrainDataIntoChunkParserAt(mapToFillDataTo, i, j); //verifies the index automatically!
        }
    }
}


void terrainCreator::applyTerrainDataIntoChunkParserAt(ChunkParserMap &mapToFillDataTo, int x, int y){

    chunk *currentChunk = chunkAt(x,y);
    if(
        (currentChunk != nullptr) && 
        (currentChunk->wasAlreadyCreated() == false)
    ){
        currentChunk->setWasCreatedTrue();

        int xLimit = map.size();
        int yLimit = map.size();

        //get chunk parser reference,
        //expects reference to be valid, and terrain generated
        //just as TerrainChunkMap given data, aswell as
        //ChunkParser Map generated in same size!
        ChunkParser &currentChunkParser = mapToFillDataTo.findByIndex(x, y);


        //apply data
        //readAndMerge (connect to next in map)
        chunk *top = chunkAt(x, y + 1);
        chunk *right = chunkAt(x + 1, y);
        chunk *topright = chunkAt(x + 1, y + 1);

        TerrainChunkSetup package = currentChunk->makeSetupPackage(top, right, topright);

        // apply position and data
        FVector newPos = currentChunk->positionPivotBottomLeft();
        currentChunkParser.createTerrainFrom2DMap(newPos, package);


        //---- CAUTION: WILL BE MOVED TO CHUNK PARSER! ----
        ETerrainType terrainType = package.getTerrainType();
        if(terrainType == ETerrainType::EOcean){
            newPos.Z = terrainConstants::HEIGHT_MAX_OCEAN * 0.8f;
            currentChunkParser.flagWaterActorNeeded(newPos);

        }

        DebugHelper::showScreenMessage("CREATED NEW CHUNK", FColor::Purple);
    }
}







void terrainCreator::randomizeTerrainTypes(){
    randomizeTerrainTypes(nullptr);
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

            chunk *currentChunk = chunkAt(xIndex, yIndex);
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
chunk *terrainCreator::chunkAt(int x, int y){
    if(verifyIndex(x) && verifyIndex(y)){
        return &map[x][y];
    }
    return nullptr;
}

TArray<chunk *> terrainCreator::chunksAt(
    TArray<FVector> &positionsWorld
){
    TArray<chunk *> outputArray;
    for (int i = 0; i < positionsWorld.Num(); i++)
    {
        FVector &current = positionsWorld[i];
        chunk *ptr = chunkAt(
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
            chunk *currentChunkPointer = chunkAt(i, j);
            if(currentChunkPointer != nullptr){
                currentChunkPointer->updateTerrainTypeBySpecialHeights();
            }
        }
    }
}

void terrainCreator::createRoads(UWorld* world){
    //roadmaker.createRoads(this, world, map.size());
}


float terrainCreator::getHeightFor(FVector2D &pos){
    FVector pos3d(pos.X, pos.Y, 0.0f);
    return getHeightFor(pos3d);
    
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






