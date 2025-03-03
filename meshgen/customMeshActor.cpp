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
    //damage owner as this could be a kimb of an actor
    if(damagedOwner != nullptr){
        damagedOwner->takedamage(d);
    }

    debugThis();

    EntityManager *entityManager = worldLevel::entityManager();
    if(entityManager != nullptr){

        // destroy if possible
        if (isDestructable())
        {
            health -= d;
            if(health <= 0){

                damagedOwner = nullptr;

                health = 100;
                if(splitOnDeath){

                    splitIntoAllTriangles();
                    // splitAndreplace(this, originPoint, 50, materialtypeSet);
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

/// @brief allows tha ctor to react to damage from a origin
/// @param d 
/// @param hitpoint hitpoint from weapon  
void AcustomMeshActor::takedamage(int d, FVector &hitpoint){
    takedamage(d);


    EntityManager *entityManager = worldLevel::entityManager();
    if(entityManager != nullptr){
        //in any case create debree
        FVector originPoint = GetActorLocation();
        entityManager->createDebree(GetWorld(), hitpoint, materialtypeSet);
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
    setMaterialBehaiviour(materialEnum::grassMaterial, false); //no split
    
    //DebugHelper::logMessage("debugCreateFoliage_terrain!");
    if(createTrees){ //debug test
        //DebugHelper::logMessage("debugCreateFoliage");
        createFoliage(touples);
    }


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






void AcustomMeshActor::filterTouplesForVerticalVectors(
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
void AcustomMeshActor::createFoliage(TArray<FVectorTouple> &touples){
    

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
    int limit = 3; //tree count
    for (int i = 0; i < limit; i++){

        int index = FVectorUtil::randomNumber(0, potentialLocations.size() - 1);
        if (index < potentialLocations.size() && index >= 0)
        {
            createTreeAndSaveToMesh(potentialLocations[index]);
            potentialLocations.erase(potentialLocations.begin() + index);
        }
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

            createNewMeshActors(allTrianglesDoubleSided, currentMaterial, false);
        }
    }

        
}

void AcustomMeshActor::createNewMeshActors(
    std::vector<MeshData> &meshes, 
    materialEnum material,
    bool splitOnDeathIn
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
            newActor->setMaterialBehaiviour(material, splitOnDeath); //split on death so kopieren!
            newActor->replaceMeshData(currentMeshData, material);
            newActor->ReloadMeshAndApplyAllMaterials();

            newActor->splitOnDeath = splitOnDeathIn;
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

void AcustomMeshActor::debugThis(){
    if(!DEBUG_enabled){
        return;
    }

    
}