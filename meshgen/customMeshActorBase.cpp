// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "p2/util/FVectorTouple.h"
#include "p2/util/AActorUtil.h"
#include "p2/pathFinding/PathFinder.h"
#include "Components/BoxComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "p2/meshgen/generation/bezierCurve.h"
#include "ELod.h"
#include "p2/meshgen/customMeshActorBase.h"

// Sets default values
AcustomMeshActorBase::AcustomMeshActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the ProceduralMeshComponent
    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
    RootComponent = Mesh;


    MeshNoRaycast = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("noRaycastMesh"));
    MeshNoRaycast->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Kein Raycast für diesen Layer

    // Attach it to the RootComponent (Mesh) so it has the same transform
    MeshNoRaycast->SetupAttachment(RootComponent);

    currentLodLevel = ELod::lodNear;
}

void AcustomMeshActorBase::enableLodListening(){
    LISTEN_FOR_LOD_PLAYER = true;


    /** 
     * ---- DEBUG! ----
     */
    LISTEN_FOR_LOD_PLAYER = false; //DISABLE, CAUSES STUTTER ON MESH CHANGE 
}

// Called when the game starts or when spawned
void AcustomMeshActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AcustomMeshActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    changeLodBasedOnPlayerPosition();
}

void AcustomMeshActorBase::disablePhysicscollision(){
    if(Mesh){
        Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}


void AcustomMeshActorBase::createTerrainFrom2DMap(
    std::vector<std::vector<FVector>> &map,
    ETerrainType typeIn
){
    thisTerrainType = typeIn;
    TArray<FVectorTouple> touples;
    createTerrainFrom2DMap(map, touples, typeIn);
}


void AcustomMeshActorBase::createTerrainFrom2DMap(
    std::vector<std::vector<FVector>> &map,
    bool foliageFlag,
    ETerrainType typeIn
){
    thisTerrainType = typeIn;
    createTerrainFrom2DMap(map, typeIn);
}

/// @brief process a 2D map of local coordinates
/// correct position of the chunk must be set before!
/// @param map 2D vector of LOCAL coordinates!
void AcustomMeshActorBase::createTerrainFrom2DMap(
    std::vector<std::vector<FVector>> &map,
	TArray<FVectorTouple> &touples, //MUST BE KEPT FOR SUBCLASS FOLIAGE CREATION!
    ETerrainType typeIn
){
    thisTerrainType = typeIn;

    int distanceBetweenNodesMin = 300;
    //MeshData &grassLayer = findMeshDataReference(materialEnum::grassMaterial, ELod::lodNear, true);
    //MeshData &stoneLayer = findMeshDataReference(materialEnum::stoneMaterial, ELod::lodNear, true);


    std::vector<FVector> navMeshAdd;
    FVector originVec(0, 0, 0);

    
    std::vector<ELod> lods = lodVector();
    int prevLodStep = 1; //x++ y++ default as expected
    for (int lodStep = 0; lodStep < lods.size(); lodStep++)
    {
        ELod lodNow = lods[lodStep];


        materialEnum groundMaterial = AcustomMeshActorBase::groundMaterialFor(typeIn);
        MeshData &grassLayer = findMeshDataReference(groundMaterial, lodNow, true);
        //MeshData &grassLayer = findMeshDataReference(materialEnum::grassMaterial, lodNow, true);
        MeshData &stoneLayer = findMeshDataReference(materialEnum::stoneMaterial, lodNow, true);

        appendLodTerrain(
            map,
            touples, // MUST BE KEPT FOR SUBCLASS FOLIAGE CREATION!
            navMeshAdd,
            grassLayer,
            stoneLayer,
            prevLodStep, // index increase
            lodStep == 0
        );
        grassLayer.calculateNormals();
        stoneLayer.calculateNormals();

        //go to next lod and clamp if needed
        prevLodStep *= 2;
        if(prevLodStep >= map.size()){
            prevLodStep = map.size() - 1;
        }
    }

    /*
    // iterate over the map and create all triangles by creating the quads from 4 given vertecies
    for (int x = 0; x < map.size() - 1; x++)
    { // -1 for inbounce connect to next
        for (int y = 0; y < map.at(x).size() - 1; y++)
        {
            // 
            //    1--2
            //    |  |
            //    0<-3
            //

            if (x + 1 < map.size() && y + 1 < map.at(x + 1).size())
            {
                try
                {
                    // get the vertecies
                    FVector vzero = map.at(x).at(y);
                    FVector vone = map.at(x).at(y + 1);
                    FVector vtwo = map.at(x + 1).at(y + 1);
                    FVector vthree = map.at(x + 1).at(y);

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

                    // calculate center
                    FVector centerLocal = FVectorUtil::calculateCenter(vzero, vone, vtwo);
                    FVector centerWorld = centerLocal + GetActorLocation();

                    // create and add touple to list
                    FVectorTouple t(centerLocal, normal); // first center, then normal
                    touples.Add(t);

                    //
                    // COLLECT NODES FOR NAV MESH
                    //

                    // testing only a few per chunk, raycasting takes a lot of time
                    if (navMeshAdd.size() <= 6 && FVectorUtil::edgeIsVertical(originVec, normal))
                    {
                        if (navMeshAdd.size() == 0)
                        {
                            navMeshAdd.push_back(centerWorld);
                        }
                        else
                        {
                            // only push nodes 3 meters away from each other -> reduce mesh count
                            FVector &prev = navMeshAdd.back();
                            if (FVector::Dist(prev, centerWorld) >= distanceBetweenNodesMin)
                            {
                                navMeshAdd.push_back(centerWorld);
                            }
                        }
                    }
                }
                catch (const std::exception &e)
                {
                    // this try catch block was just added when debugging can certainly be
                    // kept for safety
                    DebugHelper::showScreenMessage("mesh actor exception!", FColor::Red);
                }
            }
        }
    }

    grassLayer.calculateNormals();
    stoneLayer.calculateNormals();*/
    ReloadMeshAndApplyAllMaterials();

    /**
     * ADD NODES TO NAVMESH
     */

    bool addToNavMesh = true;
    if(addToNavMesh){
        double StartTime = FPlatformTime::Seconds();
        //add all normal centers to navmesh to allow the bots to move over the terrain
        if(PathFinder *f = PathFinder::instance(GetWorld())){
            FVector offset(0, 0, 70);
            f->addNewNodeVector(navMeshAdd, offset);
        }
        double EndTime = FPlatformTime::Seconds();
        double ElapsedTime = EndTime - StartTime;
        DebugHelper::addTime(ElapsedTime);
        DebugHelper::logTime("nav mesh added");
    }

    enableLodListening();
}

void AcustomMeshActorBase::appendLodTerrain(
    std::vector<std::vector<FVector>> &map,
	TArray<FVectorTouple> &touples, //MUST BE KEPT FOR SUBCLASS FOLIAGE CREATION!
    std::vector<FVector> &navMeshAdd,
    MeshData &grassLayer,
    MeshData &stoneLayer,
    int stepSize,
    bool addTouplesAndNavmeshNodes
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
                try
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


                    if(addTouplesAndNavmeshNodes){
                        // calculate center
                        FVector centerLocal = FVectorUtil::calculateCenter(vzero, vone, vtwo);
                        FVector centerWorld = centerLocal + GetActorLocation();

                        // create and add touple to list
                        FVectorTouple t(centerLocal, normal); // first center, then normal
                        touples.Add(t);

                        /**
                         * COLLECT NODES FOR NAV MESH
                        */

                        // testing only a few per chunk, raycasting takes a lot of time
                        if (navMeshAdd.size() <= 6 && FVectorUtil::edgeIsVertical(originVec, normal))
                        {
                            if (navMeshAdd.size() == 0)
                            {
                                navMeshAdd.push_back(centerWorld);
                            }
                            else
                            {
                                // only push nodes 3 meters away from each other -> reduce mesh count
                                FVector &prev = navMeshAdd.back();
                                if (FVector::Dist(prev, centerWorld) >= distanceBetweenNodesMin)
                                {
                                    navMeshAdd.push_back(centerWorld);
                                }
                            }
                        }
                    }

                    
                }
                catch (const std::exception &e)
                {
                    // this try catch block was just added when debugging can certainly be
                    // kept for safety
                    DebugHelper::showScreenMessage("mesh actor exception!", FColor::Red);
                }
            }
        }
    }
}






























/**
 * 
 * 
 * ------ NEW LOD LEVEL METHODS ------
 * 
 * 
 */



MeshData &AcustomMeshActorBase::findMeshDataReference(
    materialEnum type,
    ELod lodLevel,
    bool raycastOnLayer
){
    int layer = layerByMaterialEnum(type);
    if(raycastOnLayer){
        if(meshLayersLodMap.find(layer) == meshLayersLodMap.end()){
            meshLayersLodMap[layer] = MeshDataLod();
        }
        MeshDataLod &meshLodLevel = meshLayersLodMap[layer];
        MeshData &data = meshLodLevel.meshDataReference(lodLevel); //Alles per value irgendwo, wie es sein soll! :-)
        return data;
    }else{

        if(meshLayersLodMapNoRaycast.find(layer) == meshLayersLodMapNoRaycast.end()){
            meshLayersLodMapNoRaycast[layer] = MeshDataLod();
        }
        MeshDataLod &meshLodLevel = meshLayersLodMapNoRaycast[layer];
        MeshData &data = meshLodLevel.meshDataReference(lodLevel);
        return data;
    }
}







void AcustomMeshActorBase::changeLodBasedOnPlayerPosition(){
    /*if(!LISTEN_FOR_LOD_PLAYER){
        return;
    }*/

    referenceManager *manager = referenceManager::instance();
    if(manager != nullptr){
        FVector locationOfPlayer = manager->playerLocation();
        FVector ownLocation = GetActorLocation();

        FVector connect = ownLocation - locationOfPlayer;
        FVector playerLook = manager->playerLookDir();
        if(connect.X * playerLook.X + connect.Y * playerLook.Y > 0.0f){ //in blickrichtung auf 180 grad ebene
            updateLodLevelAndReloadMesh(
                lodLevelByDistanceTo(locationOfPlayer)
            );
        }
        
    }
}




/// @brief will update the mesh component(s) to a lower lod if the level has changed and
/// the lod listener flag is set to true
/// @param level 
void AcustomMeshActorBase::updateLodLevelAndReloadMesh(ELod level){
    if(!LISTEN_FOR_LOD_PLAYER){
        return;
    }
    if (level != currentLodLevel)
    {
        currentLodLevel = level;
        ReloadMeshAndApplyAllMaterials();
    }
}


/// @brief reloads all meshes, raycast, no raycast, all meshdata, all materials!
/// call this method when replacing mesh data or changing the lod!
void AcustomMeshActorBase::ReloadMeshAndApplyAllMaterials(){

    std::vector<materialEnum> materials = AcustomMeshActorBase::materialVector();

    //raycast
    if(Mesh){
        bool raycastOn = true;
        for (int i = 0; i < materials.size(); i++){
            int layer = layerByMaterialEnum(materials[i]);
            MeshData &meshData = findMeshDataReference(materials[i], currentLodLevel, raycastOn);
            /*
            updateMesh(
                UProceduralMeshComponent &meshcomponent,
                MeshData &otherMesh, //must be a reference which is in class scope, safe
                int layer,
                bool enableCollision
            )
            */
            updateMesh(*Mesh, meshData, layer, raycastOn);
            ApplyMaterial(materials[i]);
        }
    }


    //noraycast
    if(MeshNoRaycast){
        bool raycastOn = false;
        for (int i = 0; i < materials.size(); i++)
        {
            int layer = layerByMaterialEnum(materials[i]);
            MeshData &meshData = findMeshDataReference(materials[i], currentLodLevel, raycastOn);
            updateMesh(*MeshNoRaycast, meshData, layer, raycastOn);
            ApplyMaterialNoRaycastLayer(materials[i]);
        }
    }


}


/// @brief replaces the mesh layer for an mesh component
/// caution: mesh section is recreated because modifying the triangle buffer is not allowed
/// when will to update an mesh!
/// @param meshcomponent 
/// @param otherMesh 
/// @param layer 
/// @param enableCollision 
void AcustomMeshActorBase::updateMesh(
    UProceduralMeshComponent &meshcomponent,
    MeshData &otherMesh, //MUST BE SAVED IN A VALUE CLASS SCOPE SOMEWHERE!
    int layer,
    bool enableCollision
){
    if(otherMesh.getVerteciesRef().Num() == 0){
        return;
    }

    /**
     * example: 
     * 
    Mesh->CreateMeshSection(
        layer, 
        newvertecies, 
        this->triangles, 
        normals, 
        UV0, 
        VertexColors, 
        Tangents, 
        true
    );*/
    //meshcomponent.ClearMeshSection(layer);
    meshcomponent.CreateMeshSection(
        layer, 
        otherMesh.getVerteciesRef(),//newvertecies, 
        otherMesh.getTrianglesRef(),//this->triangles, 
        otherMesh.getNormalsRef(),//normals, 
        otherMesh.getUV0Ref(),//UV0, 
        otherMesh.getVertexColorsRef(),//VertexColors, 
        otherMesh.getTangentsRef(),//Tangents, 
        true
    );

    //set for spehere overlap
    meshcomponent.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    meshcomponent.SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    meshcomponent.SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);









    //enable if was disabled!
    //AActorUtil::showActor(*this, true);
    //AActorUtil::enableColliderOnActor(*this, true);


    if(!enableCollision){
        AActorUtil::enableColliderOnActor(*this, true);
        meshcomponent.SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }else{
        meshcomponent.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }
}


/**
 * 
 * --- helper functions for special meshes: rooms ---
 * 
 */

/// @brief adds mesh data to all lod layers (Use on custom mesh creation, for example rooms)
/// @param meshdata meshdata to add and COPY
/// @param type type of material for this layer, will be raycast enabled by default!
void AcustomMeshActorBase::replaceMeshData(MeshData &meshdata, materialEnum type){
    //all lod levels included
    std::vector<ELod> lodvector = AcustomMeshActorBase::lodVector();
    for (int i = 0; i < lodvector.size(); i++){
        replaceMeshData(meshdata, type, lodvector[i]);
    }
}

/// @brief replaces the mesh data for a lod level and a material (layer automatically found),
/// will be raycast enabled by default! (Use on custom mesh creation, for example rooms)
/// @param meshdata 
/// @param type 
/// @param lodLevel 
void AcustomMeshActorBase::replaceMeshData(MeshData &meshdata, materialEnum type, ELod lodLevel){
    int layer = layerByMaterialEnum(type);
    if (meshLayersLodMap.find(layer) == meshLayersLodMap.end())
    {
        meshLayersLodMap[layer] = MeshDataLod();
    }
    MeshDataLod &meshLodLevel = meshLayersLodMap[layer];
    meshLodLevel.replace(lodLevel, meshdata);
}










/// @brief applys a material to the whole component (slot 0 by default)
/// @param ProceduralMeshComponent 
/// @param Material 
void AcustomMeshActorBase::ApplyMaterial(UProceduralMeshComponent* ProceduralMeshComponent, UMaterial* Material) {
    ApplyMaterial(ProceduralMeshComponent, Material, 0);
}

/// @brief applys a material to the whole component at passed index slot
/// @param ProceduralMeshComponent mesh to apply for
/// @param Material material to set
/// @param layer layer to apply for / index
void AcustomMeshActorBase::ApplyMaterial(
    UProceduralMeshComponent* ProceduralMeshComponent, 
    UMaterial* Material,
    int layer
) {
	if (ProceduralMeshComponent != nullptr && Material != nullptr) {
		// Apply the material to the first material slot (index 0) of the procedural mesh
		ProceduralMeshComponent->SetMaterial(layer, Material);

    }
}







/// @brief creates a two sided quad from 4 vertecies and a material
/// expecting the vertecies to be already ordered correctly in clockwise order from a to d!
/// Will apply the mesh layer 0 and material immidiatly for this actor
/// @param a a0
/// @param b b1
/// @param c c2
/// @param d d3
/// @param material material to be applied, must not be nullptr
void AcustomMeshActorBase::createTwoSidedQuad(
    FVector &a, 
    FVector &b,
    FVector &c,
    FVector &d,
    materialEnum material
){
    MeshData meshData;
    meshData.appendDoublesided(a, b, c, d);
    meshData.calculateNormals();
    replaceMeshData(meshData, material);
    ReloadMeshAndApplyAllMaterials();
}

/// @brief applies the material to the no raycast layer as expected
/// @param type 
void AcustomMeshActorBase::ApplyMaterialNoRaycastLayer(
    materialEnum type
){
    int layer = AcustomMeshActorBase::layerByMaterialEnum(type);
    if (assetManager *e = assetManager::instance())
    {
        ApplyMaterial(MeshNoRaycast, e->findMaterial(type), layer);
    }
}



/// @brief will automatically apply the layer!
/// @param type material enum type to refresh
void AcustomMeshActorBase::ApplyMaterial(
    materialEnum type
){
    int layer = AcustomMeshActorBase::layerByMaterialEnum(type);
    if (assetManager *e = assetManager::instance())
    {
        ApplyMaterial(Mesh, e->findMaterial(type), layer);
    }
}


/// @brief returns the layer by material enum type
/// @param type type of material
/// @return int layer index
int AcustomMeshActorBase::layerByMaterialEnum(materialEnum type){
    std::vector<materialEnum> types = AcustomMeshActorBase::materialVector();
    for (int i = 0; i < types.size(); i++)
    {
        if(type == types[i]){
            return i;
        }
    }
    return 0;
}

std::vector<materialEnum> AcustomMeshActorBase::materialVector(){
    std::vector<materialEnum> types = {
        materialEnum::grassMaterial,
        materialEnum::wallMaterial,
        materialEnum::glassMaterial,
        materialEnum::stoneMaterial,
        materialEnum::sandMaterial,
        materialEnum::redsandMaterial,
        materialEnum::treeMaterial,
        materialEnum::palmLeafMaterial,
        materialEnum::waterMaterial,
        materialEnum::snowMaterial
    };
    return types;
}

materialEnum AcustomMeshActorBase::groundMaterialFor(ETerrainType terraintype){
    if(terraintype == ETerrainType::EOcean){
        return materialEnum::sandMaterial;
    }
    if(terraintype == ETerrainType::EDesert){
        return materialEnum::redsandMaterial;
    }
    if(terraintype == ETerrainType::ESnowHill){
        return materialEnum::snowMaterial;
    }
    return materialEnum::grassMaterial;
}

/**
 * 
 * 
 * -- terrain type helper --
 * 
 */
std::vector<ETerrainType> AcustomMeshActorBase::terrainVector(){
    std::vector<ETerrainType> vector = {
        ETerrainType::EDesert,
        ETerrainType::EOcean,
        ETerrainType::EForest,
        ETerrainType::ETropical,
        ETerrainType::ESnowHill
    };
    return vector;
}

/**
 * 
 * 
 * --- lod helper methods ---
 * 
 * 
 */

/// @brief creates a vector of all types from the enum in ascending order from near to far
/// @return 
std::vector<ELod> AcustomMeshActorBase::lodVector(){
    std::vector<ELod> types = {
        ELod::lodNear,
        ELod::lodMiddle,
        ELod::lodFar
    };
    return types;
}


/// @brief will check the lod and disable the mesh component if the lod is lodFar
/// @param locationOfPlayer 
/// @return 
ELod AcustomMeshActorBase::lodLevelByDistanceTo(FVector &locationOfPlayer){
    int oneMeter = 100; // 100; //WORKS GOOD WITH DISABLING ACTOR
    if (isInRange(locationOfPlayer, 100 * oneMeter))
    {
        AActorUtil::showActor(*this, true);
        return ELod::lodNear;
    }
    if(isInRange(locationOfPlayer, 200 * oneMeter)){
        AActorUtil::showActor(*this, true);
        return ELod::lodMiddle;
    }

    AActorUtil::showActor(*this, false);
    return ELod::lodFar;
}


bool AcustomMeshActorBase::isInRange(FVector &a, int maxDistance){
    FVector thisLocation = GetActorLocation();
    return (std::abs(a.X - thisLocation.X) < maxDistance) &&
           (std::abs(a.Y - thisLocation.Y) < maxDistance) &&
           (std::abs(a.Z - thisLocation.Z) < maxDistance);
}