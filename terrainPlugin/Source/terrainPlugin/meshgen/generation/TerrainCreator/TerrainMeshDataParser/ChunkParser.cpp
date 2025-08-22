
#include "ChunkParser.h"
#include "GameCore/MeshGenBase/materialHelper/MaterialEnumHelper.h"
#include "PathFinder/pathFinding/PathFinder.h"
#include "GameCore/MeshGenBase/lodHelper/LodConstants.h"

ChunkParser::ChunkParser(){

}

ChunkParser::~ChunkParser(){

}

FVector ChunkParser::GetActorLocation(){
    return actorLocation;
}

//loading interface for later
void ChunkParser::setChunkId(int inId){
    chunkId = inId;
}

int ChunkParser::getChunkId(){
    return chunkId;
}

MeshDataMap *ChunkParser::findMeshDataMap(ELod lod){
    if(meshLodContainers.find(lod) != meshLodContainers.end()){
        return &meshLodContainers[lod];
    }
    return nullptr;
}

void ChunkParser::createTerrainFrom2DMap(FVector &positionChunk, TerrainChunkSetup &package){
    actorLocation = positionChunk;
    thisTerrainType = package.getTerrainType(); // must be set before mesh gen!
    flagOutpostNeeded = package.OutPostFlagged(); //save outpost flag for later creation!

    createTerrainFrom2DMap(package.mapReference());
    
    TArray<FVectorTouple> &touples = package.freeFoliagePositionsRef();

    if(package.createTrees() && (thisTerrainType != ETerrainType::EOcean)){ 
        float percentDensity = package.treeDensitySkalar();
        createFoliageAndPushNodesAroundFoliageToNavMesh(touples, percentDensity);
    }else{
        addRandomNodesToNavmesh(touples);
    }


    //package.createOutPostIfFlagged(GetWorld());
    createBuildingIfNeeded(package);
}



/// @brief process a 2D map of local coordinates
/// correct position of the chunk must be set before!
/// @param map 2D vector of LOCAL coordinates!
void ChunkParser::createTerrainFrom2DMap(
    std::vector<std::vector<FVector>> &map
){
   

    int distanceBetweenNodesMin = 300;
    
    
    FVector originVec(0, 0, 0);

    std::vector<ELod> lods = LodConstants::lodVector();
    int prevLodStep = 1; //x++ y++ default as expected
    for (int lodStep = 0; lodStep < lods.size(); lodStep++)
    {
        ELod lodNow = lods[lodStep];


        materialEnum groundMaterial = MaterialEnumHelper::groundMaterialFor(thisTerrainType);
        MeshData &grassLayer = findMeshDataReference(groundMaterial, lodNow, true);
        MeshData &stoneLayer = findMeshDataReference(materialEnum::stoneMaterial, lodNow, true);

        appendLodTerrain(
            map,
            grassLayer,
            stoneLayer,
            prevLodStep // index increase
        );
        grassLayer.calculateNormals();
        stoneLayer.calculateNormals();

        //go to next lod and clamp if needed
        prevLodStep *= 2;
        if(prevLodStep >= map.size()){
            prevLodStep = map.size() - 1;
        }
    }


}

void ChunkParser::appendLodTerrain(
    std::vector<std::vector<FVector>> &map,
    MeshData &grassLayer,
    MeshData &stoneLayer,
    int stepSize
){
    int distanceBetweenNodesMin = 300;
    FVector originVec(0, 0, 0);

    int xstep = stepSize;
    for (int x = 0; x < map.size(); x += stepSize)
    { // -1 for inbounce connect to next

        while (x + xstep >= map.size())
        {
            xstep--;
            if(xstep <= 0){
                return;
            }
        }

        int ystep = stepSize;



        for (int y = 0; y < map.at(x).size(); y += ystep)
        {
            bool quit = false;
            while (y + ystep >= map.size())
            {
                ystep--;
                if(ystep <= 0){
                    quit = true;
                    break;
                }
            }
            if(quit){
                break;
            }
            // 
            //    1--2
            //    |  |
            //    0<-3
            //

            if (x + xstep < map.size() && y + ystep < map.at(x + xstep).size())
            {
                
                // get the vertecies
                FVector vzero = map.at(x).at(y);
                FVector vone = map.at(x).at(y + ystep);
                FVector vtwo = map.at(x + xstep).at(y + ystep);
                FVector vthree = map.at(x + xstep).at(y);

                // add to standard output
                // buildQuad(vzero, vone, vtwo, vthree, output, newtriangles);

                FVector normal = FVectorUtil::calculateNormal(vzero, vone, vtwo); // direction obviously
                if (FVectorUtil::directionIsVertical(normal))
                {
                    grassLayer.appendEfficent(vzero, vone, vtwo, vthree);
                }
                else
                {
                    stoneLayer.appendEfficent(vzero, vone, vtwo, vthree);
                }


                    
            }
        }
    }
}









void ChunkParser::filterTouplesForVerticalVectors(
    TArray<FVectorTouple> &touples,
    std::vector<FVector> &potentialLocations
){
    // iterate over touples
    // determine normal angle and apply foliage, rocks, trees accordingly
    if (touples.Num() < 1){
        return;
    }

    //if normal faces towards up: flat area, create something
    for(FVectorTouple &t : touples){
        FVector &location = t.first();
        FVector &normal = t.second();
        bool facingUpwards = FVectorUtil::directionIsVertical(normal);
        if(facingUpwards){
            potentialLocations.push_back(location); 
        }
    }
}



/// @brief create foliage and append it to the output mesh data, the output mesh data will
/// get its position from the actor. The touples expected to be in local coordinate system
/// @param touples lcoation and normal in a touple
/// @param outputAppend for example a terrain mesh to create trees on
void ChunkParser::createFoliageAndPushNodesAroundFoliageToNavMesh(
    TArray<FVectorTouple> &touples,
    float treeDensitySkalar
){
    //create trees if not in skelleton record debug mode.
    if(worldLevelBase::DebugSkelletonRecordMode()){
        return;
    }

    // iterate over touples
    // determine normal angle and apply foliage, rocks, trees accordingly
    if (touples.Num() < 1){
        return;
    }

    //saves the vertical locations to later choose random once and remove from list
    std::vector<FVector> potentialLocations;
    filterTouplesForVerticalVectors(
        touples,
        potentialLocations
    );

    //create trees at random valid locations
    std::vector<FVector> pickedLocationsForNavmesh;


    int limit = treeDensitySkalar * touples.Num();
    DebugHelper::logMessage("tree density limit: ", limit);
    //limit = 10;

    for (int i = 0; i < limit; i++){

        int index = FVectorUtil::randomNumber(0, potentialLocations.size() - 1);
        if (index < potentialLocations.size() && index >= 0)
        {
            FVector vertex = potentialLocations[index];
            pickedLocationsForNavmesh.push_back(vertex); //tree position added to navmesh
            createTreeAndSaveToMesh(vertex);
            
            
            //potentialLocations.erase(potentialLocations.begin() + index);
            potentialLocations[index] = potentialLocations.back();
            potentialLocations.pop_back();
        }
    }


    //add all points around foliage to navmesh to allow the bots to move over the terrain better
    //um um 90 grad zu drehen, x und y tauschen, einen negieren
    //(a,b) (-b,a) (-a,-b) (b, -a)
    std::vector<FVector> offsets = {
        FVector(50, 50, 70),
        FVector(-50, 50, 70),
        FVector(50, -50, 70),
        FVector(-50, -50, 70),
    };

    //f->addNewNodeVector(pickedLocationsForNavmesh, offsets);
    FVector ownLocationOffset = GetActorLocation();
    for (int i = 0; i < pickedLocationsForNavmesh.size(); i++)
    {
        std::vector<FVector> convexHull;
        FVector &currentLocation = pickedLocationsForNavmesh[i];
        for (int j = 0; j < offsets.size(); j++)
        {
            convexHull.push_back(offsets[j] + currentLocation + ownLocationOffset);
        }
        navmeshConvexHulls.push_back(convexHull);
    }

    DebugHelper::logMessage("debugPathfinder added nodes to mesh", pickedLocationsForNavmesh.size() * 4);    


}




//new!

void ChunkParser::createTreeAndSaveToMesh(FVector &location){
    
    tree.generate(thisTerrainType); 
    
    MeshData &currentTreeStemMesh = tree.meshDataStemByReference();
    MeshData &currentLeafMesh = tree.meshDataLeafByReference();

    currentTreeStemMesh.offsetAllvertecies(location);
    currentLeafMesh.offsetAllvertecies(location);

    materialEnum stemTargetMaterial = currentTreeStemMesh.targetMaterial(); //very important to have!
    materialEnum leafTargetMaterial = currentLeafMesh.targetMaterial();

    MeshData &meshDataStem = findMeshDataReference(stemTargetMaterial, ELod::lodNear, true);
    MeshData &meshDataLeaf = findMeshDataReference(leafTargetMaterial, ELod::lodNear, false); //noraycast

    meshDataStem.append(currentTreeStemMesh);
    meshDataLeaf.append(currentLeafMesh);
    
}




void ChunkParser::addRandomNodesToNavmesh(TArray<FVectorTouple> &touples){
    /**
     * ADD NODES TO NAVMESH
     */
    int size = touples.Num();
    if(size <= 0){
        return;
    }

    
    int count = touples.Num();
    std::set<int> indices;
    for (int i = 0; i < count; i++){
        int newIndex = FVectorUtil::randomNumber(0, size) % size;
        indices.insert(newIndex);
    }

    int limit = 30;
    
    std::vector<FVector> picked;
    for (auto &ref : indices)
    {
        if(ref >= 0 && ref < size){
            picked.push_back(touples[ref].first()); //first is location
            limit--;
            if(limit <= 0){
                break;
            }
        }
    }

    if(picked.size() > 0){
        navmeshNodes = picked;
    }
}






MeshData &ChunkParser::findMeshDataReference(
    materialEnum type,
    ELod lodLevel,
    bool raycastOnLayer
){
    if(meshLodContainers.find(lodLevel) == meshLodContainers.end()){
        meshLodContainers[lodLevel] = ProceduralMeshComponentPair();
    }
    return meshLodContainers[lodLevel].meshDataReference(type, raycastOnLayer);
}





void ChunkParser::addNodesToNavMeshIfNeeded(UWorld *world){
    if(navmeshNodesAdded){
        return;
    }

    // add all normal centers to navmesh to allow the bots to move over the terrain
    if (PathFinder *f = PathFinder::instance(world))
    {
        FVector offset(0, 0, 70);
        f->addNewNodeVector(navmeshNodes, offset);
        navmeshNodesAdded = true;


        //convex hulls
        for (int i = 0; i < navmeshConvexHulls.size(); i++){
            f->addConvexHull(navmeshConvexHulls[i]);
        }
    }
}




/// ----- BUILDING CREATION ------
void ChunkParser::createBuildingIfNeeded(TerrainChunkSetup &package){
    //TODO: REFACTURE BUILDING / ROOM ACTOR!
    if(package.BuildingFlagged()){
        DebugHelper::logMessage("Chunk Parser: Room Creation NOT REFACTURED");
    }
}


/// ----- WATER ACTOR FLAGS ------
void ChunkParser::flagWaterActorNeeded(FVector &location){
    waterActorNeeded = true;
    waterActorLocation = location;
}

bool ChunkParser::WaterActorNeededFlagged(){
    return waterActorNeeded;
}

FVector ChunkParser::GetWaterActorLocation(){
    return waterActorLocation;
}

bool ChunkParser::OutpostFlagCreationNeeded(){
    bool copy = flagOutpostNeeded;
    flagOutpostNeeded = false;
    return copy;
}





/// ---- API FOR APPLY DATA ----
void ChunkParser::SetUsedMeshDataByActorFlag(bool flag){
    currentlyUsedByActor = flag;
}

bool ChunkParser::IsUsedByActor(){
    return currentlyUsedByActor;
}


/// ---- API FOR STORAGE INTERFACE ----
void ChunkParser::SetActorLocation(FVector &location){
    actorLocation = location;
}
void ChunkParser::SetWaterActorNeededFlag(bool flag, FVector &location){
    waterActorLocation = location;
    waterActorNeeded = flag;
}
void ChunkParser::SetOutpostFlagNeeded(bool flag){
    flagOutpostNeeded = flag;
}
