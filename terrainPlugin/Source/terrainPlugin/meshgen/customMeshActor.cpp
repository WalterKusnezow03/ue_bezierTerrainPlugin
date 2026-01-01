// Fill out your copyright notice in the Description page of Project Settings.
#include "customMeshActor.h"

#include "CoreMinimal.h"
#include "GameCore/util/FVectorTouple.h"
#include "GameCore/util/AActorUtil.h"
#include "PathFinder/pathFinding/PathFinder.h"
#include "Components/BoxComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "terrainPlugin/meshgen/generation/bezier/bezierCurve.h"
#include "terrainPlugin/meshgen/foliage/MatrixTree/ETreeType.h"
#include "GameCore/util/FVectorUtil.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "terrainPlugin/meshgen/generation/helper/TerrainChunkSetup.h"
#include <set>
#include "GameCore/EntityGC/trackedActors.h"
#include "DebugPlugin/DebugHelper.h"
#include "GameCore/EntityGC/EntityManagerBase.h"
#include "GameCore/MeshGenBase/materialHelper/MaterialEnumHelper.h"
#include "GameCore/MeshGenBase/lodHelper/LodConstants.h"

#include "GameCore/world/worldLevelBase.h"


AcustomMeshActor* AcustomMeshActor::makeInstance(UWorld *world){
    if(world){
        UClass *toSpawn = AcustomMeshActor::StaticClass();
        if(toSpawn){
            
            FActorSpawnParameters SpawnParams;
            FVector Location;
            AcustomMeshActor *spawned = world->SpawnActor<AcustomMeshActor>(
                toSpawn, Location, FRotator::ZeroRotator, SpawnParams
            );
            return spawned;
        }
    }
    return nullptr;
}

// Sets default values
AcustomMeshActor::AcustomMeshActor() : AcustomMeshActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //terrain doesnt need much tick
    PrimaryActorTick.TickInterval = 2.0f; // Tick alle 2 sekunden

}

// Called when the game starts or when spawned
void AcustomMeshActor::BeginPlay()
{
	Super::BeginPlay();
    setTeam(teamEnum::neutralTeam); //nesecarry for being shot of everyone
    CreateFoliageInstanceComponent();
    UpdateFoliageInstanceComponent();
}

// Called every frame
void AcustomMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


/// @brief sets the health of the material 
/// if material is glass it will be split on death
/// @param mat material to set
void AcustomMeshActor::setMaterialBehaiviour(materialEnum mat){
    materialtypeSet = mat;
}

// --- derived methods from damageinferface ---

/// @brief will allow custom mesh actors such as destructables and terrain react to damage
/// @param d 
void AcustomMeshActor::takedamage(int d, bool surpressed){
    //damage owner(base method)
    Super::takedamage(d, surpressed);

    //new.
    EntityManagerBase *entityManager = EntityManagerBase::instanceBase();
    if(entityManager != nullptr){

        // destroy if possible
        if (isDestructable())
        {
            health -= d;
            if(health <= 0){
                damagedOwner = nullptr;
                health = 100;
                
                SetActorLocation(FVector(0, 0, -10000));

                //not really despawn for now
                AActorUtil::showActor(*this, false);
                AActorUtil::enableColliderOnActor(*this, false);
                entityManager->add(ETrackedActors::EMeshActor, this);

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
    groundReactionToHitWorld(hitpoint);
    glassreactionToHitWorld(hitpoint); 
    takedamage(d, surpressed);
    createDebreeOnDamage(hitpoint);

    //Ok
    DebugHelper::showScreenMessage("AcustomMeshActor damage hit registered", FColor::Orange);
}

void AcustomMeshActor::createDebreeOnDamage(FVector &worldhit){
    FVector localHit = worldToLocalHit(worldhit);

    //iterate all layers, if hit: create debree    
    std::vector<materialEnum> materials = MaterialEnumHelper::materialVector();
    for (int i = 0; i < materials.size(); i++){
        MeshData &meshdata = findMeshDataReference(
            materials[i],
            ELod::lodNear,
            true //raycast enabled
        );
        if (meshdata.doesHit(localHit)){

            //DEPREACTED WILL BE REPLACED
            /*
            EntityManager *entityManager = AworldLevel::entityManager();
            if(entityManager != nullptr){   
                entityManager->createDebree(GetWorld(), worldhit, materials[i]);
            }
            */
        }
    }
    
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



// --- CHUNK PARSER SETUP / UPDATE ---
void AcustomMeshActor::UpdateMeshDataAndPosition(ChunkParser &parser){
    //Super: std::map<ELod, ProceduralMeshComponentPair> meshLodContainers;
    SetActorLocation(parser.GetActorLocation());
    releaseChunkParserPointer();
    chunkParserPointer = &parser;

    //copy mesh data
    for(auto &pair : meshLodContainers){
        ELod ilod = pair.first;
        if(MeshDataMap *meshDataMapCache = parser.findMeshDataMap(ilod)){
            ProceduralMeshComponentPair &currentPair = pair.second;
            currentPair.overrideMeshDataFromBaseAndUpdateMesh(*meshDataMapCache);

            //needed on first launch
            currentPair.ApplyAllMaterials();

            parser.addNodesToNavMeshIfNeeded(GetWorld());
        }
    }

    //created here, not in storage based mesh.
    CreateGrassMesh();
    
    /*
    Created by actor manager already!
    if(parser.WaterActorNeededFlagged()){
        FVector waterLocation = parser.GetWaterActorLocation();
        int scaleCm = terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER;
        AcustomWaterActor::createWaterPane(
            worldPointer,
            location,
            scaleCm
        );
    }*/

    
    setMaterialBehaiviour(materialEnum::grassMaterial); //no split
    enableLodListening(); //works as expected now.

    //disableLodListening(); //debug
    // switchToLodOnBeginPlayOrUpdateMesh();
    switchToLod(currentLodLevel);

}


void AcustomMeshActor::releaseChunkParserPointer(){
    if(chunkParserPointer){
        chunkParserPointer->SetUsedMeshDataByActorFlag(false);
    }
    chunkParserPointer = nullptr;
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









void AcustomMeshActor::splitIntoAllTriangles(){
    
    
    std::vector<materialEnum> materials = MaterialEnumHelper::materialVector();
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

    EntityManagerBase *entityManagerPointer = EntityManagerBase::instanceBase();
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
        AcustomMeshActor *newActor =
            entityManagerPointer->requestByEnum<AcustomMeshActor>(
                ETrackedActors::EMeshActor,
                GetWorld()
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








void AcustomMeshActor::groundReactionToHitWorld(FVector &hitpoint){
    
    DebugHelper::showScreenMessage("AcustomMeshActor ground hit test!", FColor::Orange);

    FVector meshHit = worldToLocalHit(hitpoint);
    DebugHelper::logMessage("AcustomMeshActor mesh hit at", meshHit);

    std::vector<materialEnum> hitMaterials = {
        materialEnum::grassMaterial,
        materialEnum::redsandMaterial,
        materialEnum::sandMaterial
    };
    std::vector<ELod> lods = LodConstants::lodVector();
    int sizeHole = 200;
    FVector direction(0, 0, -200); //-20
    for (int i = 0; i < hitMaterials.size(); i++){
        for (int j = 0; j < lods.size(); j++){
            ELod lod = lods[j];
            
            MeshData &meshdata = findMeshDataReference(hitMaterials[i], lod, true);
            meshdata.pushInwards(meshHit, sizeHole, direction);
            ReloadMeshForMaterialByLod(lod, hitMaterials[i]);


            debugDrawMeshData(meshdata);
        }

        /*
        MeshData &meshdata = findMeshDataReference(hitMaterials[i], ELod::lodNear, true);
        meshdata.pushInwards(meshHit, sizeHole, direction); //error prone!
        //meshdata.cutHoleWithInnerExtensionOfMesh(localHit, sizeHole); //cut sphere
        ReloadMeshForMaterialByLod(ELod::lodNear, hitMaterials[i]);

        //debug
        debugDrawMeshData(meshdata);
        */
    }

    bool debugDrawHit = true;
    if(debugDrawHit){
        FVector up = hitpoint + FVector(0, 0, 200);
        DebugHelper::showLineBetween(
            GetWorld(),
            hitpoint,
            up,
            FColor::Purple,
            1.0f
        );
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







/// ----- foliage instancer -----
void AcustomMeshActor::CreateFoliageInstanceComponent(){
    if(!grassInstancer){
        if(UStaticMesh *mesh = StaticMeshForInstancer()){

            grassInstancer = NewObject<UMovingFoliageInstancerComponent>(this);
            int childsMax = 20 * 20;
            grassInstancer->Init(childsMax, mesh, this);
            UpdateFoliageInstanceComponent();
        }else{
            DebugHelper::logMessage("AcustomMeshActor::CreateFoliageInstanceComponent failed to load ustaticmesh");
        }
    }
}



#include "AssetPlugin/gamestart/assetManager.h"
#include "AssetPlugin/gamestart/AssetLoader.h"
#include "terrainPlugin/AssetEnums/EFoliageGrass.h"

UStaticMesh *AcustomMeshActor::StaticMeshForInstancer(){
    
    if(assetManager *instance = assetManager::instance()){
        UStaticMesh *asset = instance->Find<EFoliageGrass, UStaticMesh>(EFoliageGrass::grassAssetDefault);
        if(asset){
            return asset;
        }else{
            DebugHelper::logMessage("AcustomMeshActor asset not found");
        }
    }    
    
    
    UStaticMesh* DefaultCube = LoadObject<UStaticMesh>(
        nullptr,
        TEXT("/Engine/BasicShapes/Cube.Cube")
    );
    return DefaultCube;
}

void AcustomMeshActor::OnLodSwitch(){
    UpdateFoliageInstanceComponent();
}

void AcustomMeshActor::UpdateFoliageInstanceComponent(){
    //get grass positions
    if(grassInstancer){
        bool raycastOnLayer = true;
        MeshData &data = findMeshDataReference(
            materialEnum::grassMaterial,
            currentLodLevel, //ELod::lodNear, //kleiner test :-)
            raycastOnLayer
        );
        //const TArray<FVector> &positions = data.getVerteciesRef();
        grassInstancer->Update(
            data.getVerteciesRef(), 
            data.getNormalsRef(), 
            currentLodLevel
        );

        /*
        FString message = FString::Printf(
            TEXT("AcustomMeshActor::UpdateFoliageInstanceComponent count %d"),
            positions.Num()
        );
        DebugHelper::logMessage(message);
        DebugHelper::showScreenMessage(message);
        DebugHelper::showLineBetween(
            GetWorld(),
            GetActorLocation(),
            GetActorLocation() + FVector(0, 0, 800),
            FColor::Black,
            10.0f
        );*/
    }
}





// --- new grass ection, vertexshader based ---
void AcustomMeshActor::CreateGrassMesh(){
    if(true){
        return;
    }

    MeshData &meshFound = findMeshDataReference(
        materialEnum::grassMaterial,
        ELod::lodNear,
        true//raycastFlag
    );
    if(meshFound.hasAnyVertecies()){
        materialEnum materialLayer = materialEnum::grassMaterialWithShader;
        bool bRaycast = false;
        MeshData &grassNoRaycastLayer = findMeshDataReference(
            materialLayer,
            ELod::lodNear,
            bRaycast // no raycast
        );

        meshFound.CreateCopyRecuriveDetailTo(grassNoRaycastLayer, grassrecursion);
        //float distRecursive = 10.0f;
        //meshFound.CreateCopyRecuriveDetailToDistance(grassNoRaycastLayer, distRecursive);
        ReloadMeshForMaterialByLodAndRaycastFlag(
            ELod::lodNear,
            materialLayer,
            bRaycast
        );

        //400 -> 3000 on avg with recursion 3
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("AcustomMeshActor::madeGrass from %d to %d"),
                meshFound.verteciesNum(),
                grassNoRaycastLayer.verteciesNum()
            )
        );

        //todo:
        //new material in asset plugin - no save to storage
        //new material with shader
        //apply shader values
        //prevent saving new material


    }
}