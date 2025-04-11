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
#include "p2/util/FVectorUtil.h"
#include "p2/entities/customIk/MMatrix.h"
#include <set>
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
    materialtypeSet = mat;
    if(mat == materialEnum::glassMaterial){
        enableDebug();
    }
}




// --- derived methods from damageinferface ---

/// @brief will allow custom mesh actors such as destructables and terrain react to damage
/// @param d 
void AcustomMeshActor::takedamage(int d, bool surpressed){
    //damage owner as this could be a kimb of an actor
    if(damagedOwner != nullptr){
        damagedOwner->takedamage(d);
    }


    EntityManager *entityManager = worldLevel::entityManager();
    if(entityManager != nullptr){

        // destroy if possible
        if (isDestructable())
        {
            health -= d;
            if(health <= 0){

                damagedOwner = nullptr;

                health = 100;
                if(splitOnDeath && false){ //OLD 
                    splitIntoAllTriangles();
                }

                SetActorLocation(FVector(0, 0, -10000));

                //not really despawn for now
                AActorUtil::showActor(*this, false);
                AActorUtil::enableColliderOnActor(*this, false);
                entityManager->add(this);

            }
        }
    }
}



void AcustomMeshActor::takedamage(int d){
    takedamage(d, false);
}

/// @brief allows tha ctor to react to damage from a origin
/// @param d 
/// @param hitpoint hitpoint from weapon  
void AcustomMeshActor::takedamage(int d, FVector &hitpoint){
    takedamage(d, hitpoint, false);
}



void AcustomMeshActor::takedamage(int d, FVector &hitpoint, bool surpressed){
    debugThis(hitpoint);
    glassreactionToHitWorld(hitpoint); 
    takedamage(d, surpressed);

    /*
    EntityManager *entityManager = worldLevel::entityManager();
    if(entityManager != nullptr){
        //in any case create debree
        entityManager->createDebree(GetWorld(), hitpoint, materialtypeSet);
    }*/

    createDebreeOnDamage(hitpoint);
}

void AcustomMeshActor::createDebreeOnDamage(FVector &worldhit){
    FVector localHit = worldToLocalHit(worldhit);

    //iterate all layers, if hit: create debree
    EntityManager *entityManager = worldLevel::entityManager();
    if(entityManager != nullptr){

        //std::map<int, MeshDataLod> meshLayersLodMap;
        std::vector<materialEnum> materials = materialVector();
        for (int i = 0; i < materials.size(); i++){
            MeshData &meshdata = findMeshDataReference(
                materials[i],
                ELod::lodNear,
                true //raycast enabled
            );
            if (meshdata.doesHit(localHit)){
                entityManager->createDebree(GetWorld(), worldhit, materials[i]);
            }
        }
    }
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
    bool createTrees,
    ETerrainType typeIn
){ //nach dem entity manager stirbt die refenz hier!
    thisTerrainType = typeIn;

    TArray<FVectorTouple> touples;
    Super::createTerrainFrom2DMap(map, touples, typeIn);

    //must be called here.
    setMaterialBehaiviour(materialEnum::grassMaterial); //no split

    
    if(createTrees && (typeIn != ETerrainType::EOcean)){ 
        createFoliageAndPushNodesAroundFoliageToNavMesh(touples);
    }else{
        Super::addRandomNodesToNavmesh(touples);
    }

    enableDebug(); //DEBUG WISE FOR MESH DESTRUCTION!
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
    materialEnum type
){
    dir = dir.GetSafeNormal() * cmheight;

    FVector a1 = a + dir;
    FVector b1 = b + dir;
    FVector c1 = c + dir;
    FVector d1 = d + dir;

    createCube(a, b, c, d, a1, b1, c1, d1, type);
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
    materialEnum material
){

    MeshData cubeMesh;
    cubeMesh.appendEfficent(a, d, c, b);
    cubeMesh.appendEfficent(a1, b1, c1, d1);
    cubeMesh.appendEfficent(b, b1, a1, a);
    cubeMesh.appendEfficent(c, c1, b1, b);
    cubeMesh.appendEfficent(d, d1, c1, c);
    cubeMesh.appendEfficent(a, a1, d1, d);
    cubeMesh.calculateNormals();

    replaceMeshData(cubeMesh, material);
    ReloadMeshAndApplyAllMaterials();
}







/// @brief create foliage and append it to the output mesh data, the output mesh data will
/// get its position from the actor. The touples expected to be in local coordinate system
/// @param touples lcoation and normal in a touple
/// @param outputAppend for example a terrain mesh to create trees on
void AcustomMeshActor::createFoliageAndPushNodesAroundFoliageToNavMesh(
    TArray<FVectorTouple> &touples
){
    

    // iterate over touples
    // determine normal angle and apply foliage, rocks, trees accordingly
    if (touples.Num() < 1){
        return;
    }

    //saves the vertical locations to later choose random once and remove from list
    std::vector<FVector> potentialLocations;
    Super::filterTouplesForVerticalVectors(
        touples,
        potentialLocations
    );

    //create trees at random valid locations
    std::vector<FVector> pickedLocationsForNavmesh;


    int chunkScaleOneAxisInMeter = chunkScaleOneAxisLengthCm / 100;
    int limit = chunkScaleOneAxisInMeter / 2; // tree count
    for (int i = 0; i < limit; i++){

        int index = FVectorUtil::randomNumber(0, potentialLocations.size() - 1);
        if (index < potentialLocations.size() && index >= 0)
        {
            FVector vertex = potentialLocations[index];
            pickedLocationsForNavmesh.push_back(vertex);
            createTreeAndSaveToMesh(vertex);
            potentialLocations.erase(potentialLocations.begin() + index);
            
        }
    }


    //add all points around foliage to navmesh to allow the bots to move over the terrain better
    if (PathFinder *f = PathFinder::instance(GetWorld()))
    {
        //um um 90 grad zu drehen, x und y tauschen, einen negieren
        //(a,b) (-b,a) (-a,-b) (b, -a)
        std::vector<FVector> offsets = {
            FVector(100, 100, 70),
            FVector(-100, 100, 70),
            FVector(100, -100, 70),
            FVector(-100, -100, 70),
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
            f->addConvexHull(convexHull);
        }

        DebugHelper::logMessage("debugPathfinder added nodes to mesh", pickedLocationsForNavmesh.size() * 4);
    }

    ReloadMeshAndApplyAllMaterials();

}




//new!

void AcustomMeshActor::createTreeAndSaveToMesh(FVector &location){
    MatrixTree tree;
    tree.generate(thisTerrainType); //this terrain type now available
    
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







void AcustomMeshActor::splitIntoAllTriangles(){
    
    std::vector<MeshDataLod> newLodMeshes;
    std::vector<materialEnum> materials = materialVector();
    std::vector<bool> raycastFlags = {true, false};

    FVector actorLocation = GetActorLocation();

    for (int flag = 0; flag < raycastFlags.size(); flag++){
        bool raycastFlag = raycastFlags[flag];
        for (int i = 0; i < materials.size(); i++)
        {
            

            // only on nearest lod, because a single triangle is the simplest shape at all
            materialEnum currentMaterial = materials[i];
            MeshData &meshFound = findMeshDataReference(
                currentMaterial,
                ELod::lodNear,
                raycastFlag
            );

            std::vector<MeshData> allTrianglesDoubleSided;
            meshFound.splitAllTrianglesInHalfAndSeperateMeshIntoAllTrianglesDoubleSided(allTrianglesDoubleSided);

            createNewMeshActors(allTrianglesDoubleSided, currentMaterial);
        }
    }

        
}

void AcustomMeshActor::createNewMeshActors(
    std::vector<MeshData> &meshes, 
    materialEnum material
){

    FString message = FString::Printf(
        TEXT("DEBUGSPLIT CREATE: %d"), meshes.size()
    );
    DebugHelper::logMessage(message);

    EntityManager *entityManagerPointer = worldLevel::entityManager();
    if(entityManagerPointer == nullptr){
        return;
    }

    FVector actorLocation = GetActorLocation();
    for (int meshIndex = 0; meshIndex < meshes.size(); meshIndex++)
    {
        MeshData &currentMeshData = meshes[meshIndex];
        FVector centerOfMesh = currentMeshData.center();

        //offset lang bewegen,
        //dann vertecies des meshes zentrieren
        FVector offsetFromActorLocation = actorLocation + centerOfMesh;
        currentMeshData.centerMesh();

        // process all triangles to new actors
        AcustomMeshActor *newActor = entityManagerPointer->spawnAcustomMeshActor(
            GetWorld(), 
            offsetFromActorLocation
        );

        if (newActor != nullptr){
            newActor->setMaterialBehaiviour(material); //split on death so kopieren!
            newActor->replaceMeshData(currentMeshData, material);
            newActor->ReloadMeshAndApplyAllMaterials();
        }

        //debugDraw
        TArray<FVector> verteciesRef = currentMeshData.getVerteciesRef();
        for (int i = 0; i < verteciesRef.Num(); i++){
            DebugHelper::showLineBetween(
                GetWorld(),
                offsetFromActorLocation + verteciesRef[i],
                offsetFromActorLocation,
                FColor::Red,
                100.0f
            );
        }
    }
}






void AcustomMeshActor::enableDebug(){
    DEBUG_enabled = true;
}

FVector AcustomMeshActor::worldToLocalHit(FVector &worldhit){
    //world hit to local
    FTransform worldTransform = GetActorTransform();
    return worldTransform.InverseTransformPosition(worldhit);
}

void AcustomMeshActor::debugThis(FVector &hitpoint){
    if(!DEBUG_enabled){
        return;
    }

    FVector meshHit = worldToLocalHit(hitpoint);

    std::vector<materialEnum> hitMaterials = {
        materialEnum::grassMaterial,
        materialEnum::redsandMaterial
    };
    for (int i = 0; i < hitMaterials.size(); i++){
        MeshData &meshdata = findMeshDataReference(hitMaterials[i], ELod::lodNear, true);
        int sizeHole = 100;

        //cut push in
        FVector direction(0, 0, -25);
        meshdata.pushInwards(meshHit, sizeHole, direction); //error prone!

        //meshdata.cutHoleWithInnerExtensionOfMesh(localHit, sizeHole); //cut sphere

        ReloadMeshForMaterial(hitMaterials[i]);
    }



}



///@brief reacts to hit if has glass mesh
void AcustomMeshActor::glassreactionToHitWorld(FVector &hitpoint){
    if(hasGlassMesh()){
        //world hit to local
        FVector localHit = worldToLocalHit(hitpoint);
        glassreactionToHitLocal(localHit);
    }
}

///@brief reacts to hit if has glass mesh
void AcustomMeshActor::glassreactionToHitLocal(FVector &hitlocal){
    //if(splitOnDeath){
    if(hasGlassMesh()){
        health = 100;

        MeshData &meshFound = findMeshDataReference(
            materialEnum::glassMaterial,
            ELod::lodNear,
            true//raycastFlag
        );

        meshFound.splitAndRemoveTrianglesAt(hitlocal);
        ReloadMeshForMaterial(materialEnum::glassMaterial);
        

        debugDrawMeshData(meshFound);

        DebugHelper::showScreenMessage("glass hit!");
    }
}

bool AcustomMeshActor::hasGlassMesh(){
    MeshData &meshFound = findMeshDataReference(
        materialEnum::glassMaterial,
        ELod::lodNear,
        true//raycastFlag
    );
    return meshFound.hasAnyVertecies();
}

//Debug
void AcustomMeshActor::debugDrawMeshData(MeshData &meshdata){
    MMatrix currentTransform;

    FRotator rot = GetActorRotation();
    currentTransform.setRotation(rot);

    FVector pos = GetActorLocation();
    currentTransform.setTranslation(pos);

    meshdata.debugDrawMesh(currentTransform, GetWorld());
}

/**
 * 
 * 
 * ----- shading function for foliage ------s
 * 
 * 
 */
std::vector<materialEnum> AcustomMeshActor::foliageMaterials(){
    std::vector<materialEnum> output = {
        materialEnum::palmLeafMaterial
    };
    return output;
}
