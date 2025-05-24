// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "GameCore/util/FVectorTouple.h"
#include "GameCore/util/AActorUtil.h"
#include "Components/BoxComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "ELod.h"
#include "GameCore/MeshGenBase/lodHelper/LodCheckContainer.h"
#include "GameCore/MeshGenBase/foliage/ETerrainType.h"
#include "GameCore/util/FVectorUtil.h"
#include "GameCore/PlayerInfo/PlayerInfo.h"
#include "AssetPlugin/gameStart/assetManager.h"
#include "GameCore/MeshGenBase/customMeshActorBase.h"

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
        materialEnum::snowMaterial,
        materialEnum::beigeStoneMaterial,
        materialEnum::prop_alarmBoxMaterial,
        materialEnum::_texturedMaterial,
        materialEnum::wingMaterial
    };
    return types;
}

// Called when the game starts or when spawned
void AcustomMeshActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AcustomMeshActorBase::disableDistanceListening(){
    distanceListeningBlocked = true;
}


void AcustomMeshActorBase::enableLodListening(){
    LISTEN_FOR_LOD_PLAYER = true;


    /** 
     * ---- DEBUG! ----
     */
    LISTEN_FOR_LOD_PLAYER = false; //DISABLE, CAUSES STUTTER ON MESH CHANGE 
}


// Called every frame
void AcustomMeshActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    TickShaderRunningTime(DeltaTime);
    changeLodBasedOnPlayerPosition();

    
}

void AcustomMeshActorBase::disablePhysicscollision(){
    if(Mesh){
        Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}





/// @brief process a 2D map of local coordinates
/// correct position of the chunk must be set before!
/// @param map 2D vector of LOCAL coordinates!
void AcustomMeshActorBase::createTerrainFrom2DMap(
    std::vector<std::vector<FVector>> &map,
    ETerrainType typeIn
){
    thisTerrainType = typeIn;

    int distanceBetweenNodesMin = 300;
    //MeshData &grassLayer = findMeshDataReference(materialEnum::grassMaterial, ELod::lodNear, true);
    //MeshData &stoneLayer = findMeshDataReference(materialEnum::stoneMaterial, ELod::lodNear, true);


    
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


    //find chunk scale for foliage process
    if(map.size() > 0 && map[0].size() > 0){
        FVector &a = map[0][0];
        FVector &b = map[0].back();
        chunkScaleOneAxisLengthCm = std::abs(FVector::Dist(a, b));
    }


    ReloadMeshAndApplyAllMaterials();

    enableLodListening();
}

void AcustomMeshActorBase::appendLodTerrain(
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









void AcustomMeshActorBase::filterTouplesForVerticalVectors(
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








/**
 * 
 * 
 * ------ NEW LOD LEVEL METHODS ------
 * 
 * 
 */
MeshData &AcustomMeshActorBase::findMeshDataReference(
    materialEnum type,
    bool raycastOnLayer
){
    
    return findMeshDataReference(
        type,
        ELod::lodNear,
        raycastOnLayer
    );
}

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







///@brief will append the meshdata AND RELOAD THE MESH
void AcustomMeshActorBase::appendMeshDataAndReload(
    MeshData &meshdata,
    materialEnum type,
    ELod lodLevel,
    bool raycastOnLayer
){
    MeshData &found = findMeshDataReference(type, lodLevel, raycastOnLayer);
    found.append(meshdata);
    ReloadMeshForMaterial(type);
    // ReloadMeshAndApplyAllMaterials();
}

void AcustomMeshActorBase::changeLodBasedOnPlayerPosition(){
    /*if(!LISTEN_FOR_LOD_PLAYER){
        return;
    }*/



    FVector locationOfPlayer = PlayerInfo::playerLocation();
    FVector playerLook = PlayerInfo::playerLookDir();
    FVector ownLocation = GetActorLocation();

    FVector connect = ownLocation - locationOfPlayer;
    float dotLookDir = connect.X * playerLook.X + connect.Y * playerLook.Y;


    this->SetActorHiddenInGame(false);  // macht den Actor unsichtbar
    if(dotLookDir > 0.0f){ //in blickrichtung auf 180 grad ebene

        //set invisble
        ELod lod = lodLevelByDistanceTo(locationOfPlayer);
        
        /*
        if(lod == ELod::lodFar){
            this->SetActorHiddenInGame(true); 
            return;
        }else{
            updateLodLevelAndReloadMesh(lod);
        }*/
        
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
    for (int i = 0; i < materials.size(); i++){
        ReloadMeshForMaterial(materials[i]);
    }
}

///@brief reloads the mesh data for a single material
void AcustomMeshActorBase::ReloadMeshForMaterial(materialEnum material){
    //raycast
    if(Mesh){
        bool raycastOn = true;
        int layer = layerByMaterialEnum(material);
        MeshData &meshData = findMeshDataReference(material, currentLodLevel, raycastOn);
        updateMesh(*Mesh, meshData, layer, raycastOn);
        ApplyMaterial(material);
    }


    //noraycast
    if(MeshNoRaycast){
        bool raycastOn = false;
        int layer = layerByMaterialEnum(material);
        MeshData &meshData = findMeshDataReference(material, currentLodLevel, raycastOn);
        updateMesh(*MeshNoRaycast, meshData, layer, raycastOn);
        ApplyMaterialNoRaycastLayer(material);
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

    
    //meshcomponent.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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
void AcustomMeshActorBase::ApplyMaterial(UProceduralMeshComponent* ProceduralMeshComponent, 
    UMaterialInterface* Material
) {
    ApplyMaterial(ProceduralMeshComponent, Material, 0);
}

/// @brief applys a material to the whole component at passed index slot
/// @param ProceduralMeshComponent mesh to apply for
/// @param Material material to set
/// @param layer layer to apply for / index
void AcustomMeshActorBase::ApplyMaterial(
    UProceduralMeshComponent* ProceduralMeshComponent, 
    UMaterialInterface* Material,
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
    if(terraintype == ETerrainType::EDesertForest){
        return materialEnum::beigeStoneMaterial;
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
        ETerrainType::ESnowHill,
        ETerrainType::EDesertForest
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
        ELod::lodFar
    };
    return types;
}


///@brief will find the LOD distance to the given position and disable the actor if lod is far
ELod AcustomMeshActorBase::lodLevelByDistanceTo(FVector &locationOfPlayer){

    FVector a = GetActorLocation();
    LodCheckContainer checkContainer(a, locationOfPlayer);
    if(!distanceListeningBlocked){
        SetActorHiddenInGame(checkContainer.hideActorByLod());
    }
    

    return checkContainer.lod();
}

/**
 * 
 * 
 * --- refresh mesh for dynamic actors like water or other moving meshes ---
 * 
 * 
 */
void AcustomMeshActorBase::TickShaderRunningTime(float DeltaTime){
    shaderRunningTime += DeltaTime;
    if(shaderRunningTime > 2 * M_PI){
        shaderRunningTime = 0.0f;
    }
}

void AcustomMeshActorBase::vertexShaderFor(MeshData &data){
    TArray<FVector> &vertecies = data.getVerteciesRef();
    for (int i = 0; i < vertecies.Num(); i++){
        FVector &current = vertecies[i];
        applyShaderToVertex(current);
    }
}

/// @brief apply vertex shader to the given vertex
/// @param vertex vertex to move
void AcustomMeshActorBase::applyShaderToVertex(FVector &vertex){
    //keep empty, is virtaul here.
}

void AcustomMeshActorBase::refreshMesh(
    UProceduralMeshComponent& meshComponent,
    MeshData &other,
    int layer
){
    if(other.verteciesNum() <= 0){
        return;
    }

    meshComponent.UpdateMeshSection(
        layer, 
        other.getVerteciesRef(), 
        other.getNormalsRef(), 
        other.getUV0Ref(),
        other.getVertexColorsRef(), 
        other.getTangentsRef()
    );

    if(false){
        meshComponent.SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        meshComponent.SetCollisionResponseToAllChannels(ECR_Ignore);
        meshComponent.SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    }

}





/**
 * 
 * 
 * mesh hit test
 * 
 * 
 */

FVector AcustomMeshActorBase::worldToLocalHit(FVector &worldhit){
    //world hit to local
    FTransform worldTransform = GetActorTransform();
    return worldTransform.InverseTransformPosition(worldhit);
}


bool AcustomMeshActorBase::doesHitWorld(FVector &hitWorld, materialEnum mat){
    FVector hitLocal = worldToLocalHit(hitWorld);
    return doesHitLocal(hitLocal, mat);
}

bool AcustomMeshActorBase::doesHitLocal(FVector &hitLocal, materialEnum mat){
    MeshData &meshFound = findMeshDataReference(
        mat,
        ELod::lodNear,
        true//raycastFlag
    );
    return meshFound.doesHit(hitLocal);
}

