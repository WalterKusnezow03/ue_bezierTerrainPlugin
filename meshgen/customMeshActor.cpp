// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "p2/util/FVectorTouple.h"
#include "p2/util/AActorUtil.h"
#include "p2/pathFinding/PathFinder.h"
#include "Components/BoxComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "p2/meshgen/generation/bezierCurve.h"
#include "p2/meshgen/foliage/MatrixTree.h"
#include "p2/meshgen/foliage/ETreeType.h"
#include "customMeshActor.h"


// Sets default values
AcustomMeshActor::AcustomMeshActor() : AcustomMeshActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AcustomMeshActor::BeginPlay()
{
	Super::BeginPlay();
    setTeam(teamEnum::neutralTeam); //nesecarry for being shot of everyone
    
}

// Called every frame
void AcustomMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AcustomMeshActor::setDamagedOwner(IDamageinterface *damagedOwnerIn){
    damagedOwner = damagedOwnerIn;
}

/// @brief sets the health of the material 
/// if material is glass it will be split on death
/// @param mat material to set
void AcustomMeshActor::setMaterialBehaiviour(materialEnum mat){
    if(mat == materialEnum::glassMaterial){
        setMaterialAndHealthAndSplitOnDeath(mat, 1, true);
    }else{
        setMaterialBehaviourAndHealth(mat, 100);
    }
}

/// @brief sets the material and bool of splitting on death, healt managed automatically
/// @param mat material to set
/// @param split split
void AcustomMeshActor::setMaterialBehaiviour(materialEnum mat, bool split){
    bool isGlass = (mat == materialEnum::glassMaterial);
    int healthIn = isGlass ? 1 : 100;
    setMaterialAndHealthAndSplitOnDeath(mat, healthIn, split);
}

void AcustomMeshActor::setMaterialBehaviourAndHealth(materialEnum mat, int healthIn){
    setMaterialAndHealthAndSplitOnDeath(mat, healthIn, false);
}

void AcustomMeshActor::setMaterialAndHealthAndSplitOnDeath(materialEnum mat, int healthIn, bool split){
    materialtypeSet = mat;
    setHealth(std::max(1, healthIn));
    splitOnDeath = split;
}



// --- derived methods from damageinferface ---

/// @brief will allow custom emsh actors such as destructables and terrain react to damage
/// @param d 
void AcustomMeshActor::takedamage(int d){

    EntityManager *entityManager = worldLevel::entityManager();

    if(entityManager != nullptr){
        //in any case create debree?

        FVector originPoint = GetActorLocation();
        entityManager->createDebree(GetWorld(), originPoint, materialtypeSet);

        // destroy if possible
        if (isDestructable())
        {
            health -= d;
            if(health <= 0){

                damagedOwner = nullptr;

                health = 100;
                if(splitOnDeath){
                    splitAndreplace(this, originPoint, 50, materialtypeSet);
                }

                SetActorLocation(FVector(0, 0, -10000));

                //not really despawn for now
                AActorUtil::showActor(*this, false);
                AActorUtil::enableColliderOnActor(*this, false);
            
                if(entityManager != nullptr){
                    entityManager->add(this);
                }
            }
        }
    }

    
    if(damagedOwner != nullptr){
        damagedOwner->takedamage(d);
    }
}

/// @brief allows tha ctor to react to damage from a origin
/// @param d 
/// @param from 
void AcustomMeshActor::takedamage(int d, FVector &from){
    takedamage(d);

    //angle of debree might be calculated from angle to normal for example

}

void AcustomMeshActor::setTeam(teamEnum t){
    this->team = t;
}
teamEnum AcustomMeshActor::getTeam(){
    return team;
}

void AcustomMeshActor::setHealth(int d){
    if(d <= 0){
        health = 1;
        return;
    }
    health = d;
}

// --- mesh actor methods ---

/// @brief will check if the mesh is fully destructable by type
/// @return true false
bool AcustomMeshActor::isDestructable(){
    bool properMaterial = materialtypeSet == materialEnum::glassMaterial ||
           materialtypeSet == materialEnum::wallMaterial;

    if(damagedOwner != nullptr){
        return false;
    }

    return properMaterial;
}






/// @brief process a 2D map of local coordinates
/// correct position of the chunk must be set before!
/// @param map 2D vector of LOCAL coordinates!
void AcustomMeshActor::createTerrainFrom2DMap(
    std::vector<std::vector<FVector>> &map,
    bool createTrees    
){ //nach dem entity manager stirbt die refenz hier!

    TArray<FVectorTouple> touples;
    Super::createTerrainFrom2DMap(map, touples);

    //must be called here.
    setMaterialBehaiviour(materialEnum::grassMaterial, false); //no split
    
    DebugHelper::logMessage("debugCreateFoliage_terrain!");
    if(createTrees){
        DebugHelper::logMessage("debugCreateFoliage");
        createFoliage(touples);
    }


}




/// @brief processes a 2D and writes the mesh data into the given output object
/// @param map map to process
/// @param outputData output to write
void AcustomMeshActor::process2DMapSimple(
    std::vector<std::vector<FVector>> &map,
    MeshData &outputData
){ 
    //just one layer
    TArray<FVector> output_layer;
    TArray<int32> triangles_layer;

    FVector originVec(0, 0, 0);

    //iterate over the map and create all triangles by creating the quads from 4 given vertecies
    for (int x = 0; x < map.size() - 1; x++){
        for (int y = 0; y < map.at(x).size() - 1; y++){
            /*
                1--2
                |  |
                0<-3
             */
            bool copy = (x != 0); //prev 0 and 1 indices will be copied


            if(x + 1 < map.size() && y + 1 < map.at(x + 1).size()){
                try{
                    //get the vertecies
                    FVector &vzero = map.at(x).at(y);
                    FVector &vone = map.at(x).at(y + 1);
                    FVector &vtwo = map.at(x + 1).at(y + 1);
                    FVector &vthree = map.at(x + 1).at(y);
                    buildQuad(vzero, vone, vtwo, vthree, output_layer, triangles_layer);

                }
                catch (const std::exception &e)
                {
                    //this try catch block was just added when debugging can certainly be
                    //kept for safety 
                    DebugHelper::showScreenMessage("mesh actor exception!", FColor::Red);
                }
            }
            
        }
    }

    outputData.rebuild(MoveTemp(output_layer), MoveTemp(triangles_layer));

}









/// @brief creates a cube from 4 vertecies and a material
/// expecting the vertecies to be already ordered correctly in clockwise order from a to d!
/// expecting a to d to be the bottom side and the direction to be positive!
/// @param a a0
/// @param b b1
/// @param c c2
/// @param d d3
/// @param dir direction to extend
/// @param cmheight height of the shape / direction length
void AcustomMeshActor::createCube(
    FVector &a, 
    FVector &b,
    FVector &c,
    FVector &d,
    FVector &dir,
    int cmheight,
    UMaterial *material
){
    if(material != nullptr){

        dir = dir.GetSafeNormal() * cmheight;

        FVector a1 = a + dir;
        FVector b1 = b + dir;
        FVector c1 = c + dir;
        FVector d1 = d + dir;

        createCube(a, b, c, d, a1, b1, c1, d1, material);


    }
}

/// @brief expect a -d to be clockwise bottom quad and a1-d1 to be clockwise top quad
/// @param a 
/// @param b 
/// @param c 
/// @param d 
/// @param a1 
/// @param b1 
/// @param c1 
/// @param d1 
void AcustomMeshActor::createCube(
    FVector &a, 
    FVector &b,
    FVector &c,
    FVector &d,
    FVector &a1, 
    FVector &b1,
    FVector &c1,
    FVector &d1,
    UMaterial *material
){

    MeshData meshDataOutput;
    createQuad(a, d, c, b, meshDataOutput);
    createQuad(a1, b1, c1, d1, meshDataOutput);

    //sides
    //must be reverse winding order (ccl)
    //instead of 0123 -> 3210 to be flipped correctly!
    createQuad(b, b1, a1, a, meshDataOutput); //correct, must be reverse winding order (ccl)
    createQuad(c, c1, b1, b, meshDataOutput); 
    createQuad(d, d1, c1, c, meshDataOutput);
    createQuad(a, a1, d1, d, meshDataOutput);
    

    //AcustomMeshActor::updateMesh(MeshData otherMesh, bool createNormals, int layer)
    updateMesh(meshDataOutput, false, 0);
    if(material != nullptr){
        ApplyMaterial(Mesh, material);
    }else{
        //find wall material if none was provided / nullptr
        if(assetManager *e = assetManager::instance()){
            ApplyMaterial(Mesh, e->findMaterial(materialEnum::wallMaterial));
        }
    }


}




/// @brief creates a cube mesh from  vertecies, a extend direction and extend length
/// the mesh data will  be written into the pass by ref output object
/// @param a 
/// @param b 
/// @param c 
/// @param d 
/// @param a1 
/// @param b1 
/// @param c1 
/// @param d1 
/// @param meshDataOutput 
void AcustomMeshActor::createCube(
    FVector &a, 
    FVector &b,
    FVector &c,
    FVector &d,
    FVector &a1, 
    FVector &b1,
    FVector &c1,
    FVector &d1,
    MeshData &meshDataOutput
){
    createQuad(a, d, c, b, meshDataOutput);
    createQuad(a1, b1, c1, d1, meshDataOutput);

    //sides
    //must be reverse winding order (ccl)
    //instead of 0123 -> 3210 to be flipped correctly!
    createQuad(b, b1, a1, a, meshDataOutput); //correct, must be reverse winding order (ccl)
    createQuad(c, c1, b1, b, meshDataOutput); 
    createQuad(d, d1, c1, c, meshDataOutput);
    createQuad(a, a1, d1, d, meshDataOutput);
}


/// @brief creates a cube mesh from  vertecies, a extend direction and extend length
/// the mesh data will  be written into the pass by ref output object
/// @param a 
/// @param b 
/// @param c 
/// @param d 
/// @param dir direction of extend
/// @param cmheight cm extend to have
/// @param outputMeshData mesh data to write to
void AcustomMeshActor::createCube(
	FVector &a,
	FVector &b,
	FVector &c,
	FVector &d,
	FVector &dir,
	int cmheight,
	MeshData &outputMeshData
){
    dir = dir.GetSafeNormal() * cmheight;
    FVector a1 = a + dir;
    FVector b1 = b + dir;
    FVector c1 = c + dir;
    FVector d1 = d + dir;
    createCube(a, b, c, d, a1, b1, c1, d1, outputMeshData);
}











/// @brief create foliage and append it to the output mesh data, the output mesh data will
/// get its position from the actor. The touples expected to be in local coordinate system
/// @param touples lcoation and normal in a touple
/// @param outputAppend for example a terrain mesh to create trees on
void AcustomMeshActor::createFoliage(TArray<FVectorTouple> &touples){
    MeshData meshDataStem;
    MeshData meshDataLeaf;

    // iterate over touples
    // determine normal angle and apply foliage, rocks, trees accordingly
    if (touples.Num() < 1){
        return;
    }


    //saves the vertical locations to later choose random once and remove from list
    std::vector<FVector> potentialLocations;

    //if normal faces towards up: flat area, create something
    for(FVectorTouple &t : touples){
        FVector &location = t.first();
        FVector &normal = t.second();
        bool facingUpwards = FVectorUtil::directionIsVertical(normal);
        if(facingUpwards){
            potentialLocations.push_back(location); 
        }
    }

    //create trees at random valid locations
    int limit = 3; //tree count
    for (int i = 0; i < limit; i++){

        int index = FVectorUtil::randomNumber(0, potentialLocations.size() - 1);
        if (index < potentialLocations.size() && index >= 0)
        {
            createTreeAndSaveMeshTo(potentialLocations[index], meshDataStem, meshDataLeaf);
            potentialLocations.erase(potentialLocations.begin() + index);
        }
    }


    //updateMeshNoRaycastLayer(meshDataStem, false, layerByMaterialEnum(materialEnum::treeMaterial));

    updateMesh(meshDataStem, false, layerByMaterialEnum(materialEnum::treeMaterial));
    ApplyMaterial(materialEnum::treeMaterial);

    //different layer for meshes, no raycast / physics
    updateMeshNoRaycastLayer(meshDataLeaf, false, layerByMaterialEnum(materialEnum::palmLeafMaterial));
    ApplyMaterialNoRaycastLayer(materialEnum::palmLeafMaterial);

}



void AcustomMeshActor::createTreeAndSaveMeshTo(
    FVector &location, 
    MeshData &meshDataStem, 
    MeshData &meshDataLeaf
){
    MatrixTree tree;
    tree.generate(ETerrainType::ETropical); //1000 height, 100 step per matrix
    
    MeshData &currentTreeStemMesh = tree.meshDataStemByReference();
    MeshData &currentLeafMesh = tree.meshDataLeafByReference();

    currentTreeStemMesh.offsetAllvertecies(location);
    currentLeafMesh.offsetAllvertecies(location);

    meshDataStem.append(currentTreeStemMesh);
    meshDataLeaf.append(currentLeafMesh);
}

/// @brief will replace the actor and split it (int terms of bounds) and apply an material
/// will not use original mesh, just the bounds
/// @param actor actor to replace with splitted mesh
/// @param bottomCenter bottom center of the actor, very important, do not ignore
/// @param cmTile each tile to be cm wide and high
/// @param material material to set for the mesh
void AcustomMeshActor::splitAndreplace(
    AActor *actor,
    FVector &bottomCenter,
    int cmTile,
    materialEnum materialType
)
{
    
    UMaterial *material = nullptr;
    assetManager *am = assetManager::instance();
    if(am != nullptr){
        material = am->findMaterial(materialType);
    }

    if(actor != nullptr && material != nullptr){

        int xBound = 0;
        int yBound = 0;
        int zBound = 0;
        AActorUtil::calculateActorBounds(actor, xBound, yBound, zBound);


        //create a mesh where the pieces are almost a similar side viewed from the larger side
        //like consitent quads

        EntityManager *eM = worldLevel::entityManager();
        if (eM != nullptr)
        {
            //bottom left corner
            FVector anchor = bottomCenter;
            anchor.X -= xBound / 2; //bottom left now (bounds adjusted half way obviosuly)
            anchor.Y -= yBound / 2;

            FVector up(0, 0, cmTile); //always in up direction for now
            FVector side = (xBound > yBound) ? FVector(cmTile, 0, 0) : FVector(0, cmTile, 0); //iterate along longer
            FVector extension = (xBound > yBound) ? FVector(0, yBound, 0) : FVector(xBound, 0, 0);  //90 degree to longer
            

            
            float tilesXAxis = (xBound > yBound) ? xBound / cmTile : yBound / cmTile;
            float tilesZAxis = zBound / cmTile;
            if(tilesXAxis < 1){
                side *= tilesXAxis; //scale accordinly down!
                tilesXAxis = 1; // set to one to still iterate once
            }
            if(tilesZAxis < 1){
                up *= tilesZAxis; //scale accordinly down!
                tilesZAxis = 1; // set to one to still iterate once
            }
            

            //DEBUGGING NEEDED

            //new: symetrical splitting, randomize offset, create meshes
            std::vector<std::vector<FVector>> splitted;
            for (int i = 0; i <= tilesXAxis; i++)
            {
                std::vector<FVector> positions;
                for (int j = 0; j <= tilesZAxis; j++)
                {
                    //create the grid properly
                    FVector pos = i * side + j * up;
                    positions.push_back(pos);
                }
                splitted.push_back(positions);
            }

            //random offsets
            for (int i = 1; i < splitted.size() - 1; i++)
            {
                for (int j = 1; j < splitted[i].size() - 1; j++){ //only inner vertecies to offset
                    // scale from 0 to 1
                    FVector offset = side * FVectorUtil::randomFloatNumber(-1.0f, 1.0f) * 0.5f;
                    splitted[i][j] += offset;
                }
            }
            //create cubes from touples like 2d map
            for (int i = 0; i < splitted.size() - 1; i++)
            {
                for (int j = 0; j < splitted[i].size() - 1; j++){

                    FVector center = anchor + splitted[i][j];
                    AcustomMeshActor *newActor = eM->spawnAcustomMeshActor(actor->GetWorld(), center);
                    if (newActor != nullptr){
                        /*
                        1  2

                        0  3
                        */
                        FVector &t0_base = splitted[i][j];
                        FVector t0(0, 0, 0);
                        FVector t1 = splitted[i][j+1] - t0_base;
                        FVector t2 = splitted[i+1][j+1] - t0_base;
                        FVector t3 = splitted[i+1][j] -t0_base;

                        FVector t0a = t0 + extension;
                        FVector t1a = t1 + extension;
                        FVector t2a = t2 + extension;
                        FVector t3a = t3 + extension;

                        DebugHelper::showLineBetween(actor->GetWorld(), center + t0, center + t1);
                        DebugHelper::showLineBetween(actor->GetWorld(), center + t1, center + t2);
                        DebugHelper::showLineBetween(actor->GetWorld(), center + t2, center + t3);
                        DebugHelper::showLineBetween(actor->GetWorld(), center + t3, center + t0);

                        // init on begin!
                        bool split = false;
                        newActor->setMaterialBehaiviour(materialType, split);
                        // newActor->init(materialType);

                        // apply mesh
                        // is correct like this, do not touch
                        newActor->createCube(
                            t0, t1, t2, t3,     //bottom quad clw
                            t0a, t1a, t2a, t3a, //top quad clw
                            material
                        );
                        
                    }
                }
            }

        }
    }
}








/// @brief will replace the actor and split it (int terms of bounds) and apply an material
/// will not use original mesh, just the bounds
/// @param actor actor to replace with splitted mesh
/// @param bottomCenter bottom center of the actor, very important, do not ignore
/// @param cmTile each tile to be cm wide and high
/// @param material material to set for the mesh
void AcustomMeshActor::splitAndreplace(
    FVector &bottomCenter,
    int xBound,
    int yBound,
    int zBound,
    int cmTile,
    materialEnum materialType,
    UWorld *world
)
{
    
    UMaterial *material = nullptr;
    assetManager *am = assetManager::instance();
    if(am != nullptr){
        material = am->findMaterial(materialType);
    }

    if(material != nullptr){

        //create a mesh where the pieces are almost a similar side viewed from the larger side
        //like consitent quads

        EntityManager *eM = worldLevel::entityManager();
        if (eM != nullptr)
        {
            //bottom left corner
            FVector anchor = bottomCenter;
            anchor.X -= xBound / 2; //bottom left now (bounds adjusted half way obviosuly)
            anchor.Y -= yBound / 2;

            FVector up(0, 0, cmTile); //always in up direction for now
            FVector side = (xBound > yBound) ? FVector(cmTile, 0, 0) : FVector(0, cmTile, 0); //iterate along longer
            FVector extension = (xBound > yBound) ? FVector(0, yBound, 0) : FVector(xBound, 0, 0);  //90 degree to longer
            

            
            float tilesXAxis = (xBound > yBound) ? xBound / cmTile : yBound / cmTile;
            float tilesZAxis = zBound / cmTile;
            if(tilesXAxis < 1){
                side *= tilesXAxis; //scale accordinly down!
                tilesXAxis = 1; // set to one to still iterate once
            }
            if(tilesZAxis < 1){
                up *= tilesZAxis; //scale accordinly down!
                tilesZAxis = 1; // set to one to still iterate once
            }
            

            //DEBUGGING NEEDED

            //new: symetrical splitting, randomize offset, create meshes
            std::vector<std::vector<FVector>> splitted;
            for (int i = 0; i <= tilesXAxis; i++)
            {
                std::vector<FVector> positions;
                for (int j = 0; j <= tilesZAxis; j++)
                {
                    //create the grid properly
                    FVector pos = i * side + j * up;
                    positions.push_back(pos);
                }
                splitted.push_back(positions);
            }

            //random offsets
            for (int i = 1; i < splitted.size() - 1; i++)
            {
                for (int j = 1; j < splitted[i].size() - 1; j++){ //only inner vertecies to offset
                    // scale from 0 to 1
                    FVector offset = side * FVectorUtil::randomFloatNumber(-1.0f, 1.0f) * 0.5f;
                    splitted[i][j] += offset;
                }
            }
            //create cubes from touples like 2d map
            for (int i = 0; i < splitted.size() - 1; i++)
            {
                for (int j = 0; j < splitted[i].size() - 1; j++){

                    FVector center = anchor + splitted[i][j];
                    AcustomMeshActor *newActor = eM->spawnAcustomMeshActor(world, center);
                    if (newActor != nullptr){
                        /*
                        1  2

                        0  3
                        */
                        FVector &t0_base = splitted[i][j];
                        FVector t0(0, 0, 0);
                        FVector t1 = splitted[i][j+1] - t0_base;
                        FVector t2 = splitted[i+1][j+1] - t0_base;
                        FVector t3 = splitted[i+1][j] -t0_base;

                        FVector t0a = t0 + extension;
                        FVector t1a = t1 + extension;
                        FVector t2a = t2 + extension;
                        FVector t3a = t3 + extension;

                        // init on begin!
                        bool split = false;
                        newActor->setMaterialBehaiviour(materialType, split);
                        // newActor->init(materialType);

                        // apply mesh
                        // is correct like this, do not touch
                        newActor->createCube(
                            t0, t1, t2, t3,     //bottom quad clw
                            t0a, t1a, t2a, t3a, //top quad clw
                            material
                        );
                        
                    }
                }
            }

        }
    }
}










