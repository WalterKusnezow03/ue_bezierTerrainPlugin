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

// Called when the game starts or when spawned
void AcustomMeshActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AcustomMeshActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




void AcustomMeshActorBase::createTerrainFrom2DMap(
    std::vector<std::vector<FVector>> &map
){
	TArray<FVectorTouple> touples;
	createTerrainFrom2DMap(map, touples);
}


void AcustomMeshActorBase::createTerrainFrom2DMap(
    std::vector<std::vector<FVector>> &map,
    bool foliageFlag
){
    createTerrainFrom2DMap(map);
}

/// @brief process a 2D map of local coordinates
/// correct position of the chunk must be set before!
/// @param map 2D vector of LOCAL coordinates!
void AcustomMeshActorBase::createTerrainFrom2DMap(
    std::vector<std::vector<FVector>> &map,
	TArray<FVectorTouple> &touples 
){ //nach dem entity manager stirbt die refenz hier!

    
    //new refacturing
    /*MeshData grassLayer;
    MeshData stoneLayer;*/

    MeshData &grassLayer = findMeshDataReference(materialEnum::grassMaterial, ELod::lodNear, true);
    MeshData &stoneLayer = findMeshDataReference(materialEnum::stoneMaterial, ELod::lodNear, true);


    std::vector<FVector> navMeshAdd;

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
                    FVector vzero = map.at(x).at(y);
                    FVector vone = map.at(x).at(y + 1);
                    FVector vtwo = map.at(x + 1).at(y + 1);
                    FVector vthree = map.at(x + 1).at(y);

                    //add to standard output
                    //buildQuad(vzero, vone, vtwo, vthree, output, newtriangles);

                    FVector normal = FVectorUtil::calculateNormal(vzero, vone, vtwo); //direction obviously
                    if(FVectorUtil::directionIsVertical(normal)){
                        //add to standard output, if direction of normal is vertical, the pane is flat

                        //grassLayer.append(vzero, vone, vtwo, vthree);

                        grassLayer.appendEfficent(vzero, vone, vtwo, vthree); 
                    }
                    else
                    {
                        //otherwise the quad should be added to the second
                        //triangle / vertecy array for stone material, more vertical
                        
                        //stoneLayer.append(vzero, vone, vtwo, vthree);

                        stoneLayer.appendEfficent(vzero, vone, vtwo, vthree); 
                    }

                    //calculate center
                    FVector centerLocal = FVectorUtil::calculateCenter(vzero, vone, vtwo);
                    FVector centerWorld = centerLocal + GetActorLocation();
                   

                    // create and add touple to list
                    FVectorTouple t(centerLocal, normal); // first center, then normal
                    touples.Add(t);

                    

                    /**
                     * COLLECT NODES FOR NAV MESH
                     */

                    //testing only a few per chunk, raycasting takes a lot of time
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
                            if (FVector::Dist(prev, centerWorld) >= 300)
                            {
                                navMeshAdd.push_back(centerWorld);
                            }
                        }
                    }
                }catch (const std::exception &e)
                {
                    //this try catch block was just added when debugging can certainly be
                    //kept for safety 
                    DebugHelper::showScreenMessage("mesh actor exception!", FColor::Red);
                }
            }
            
        }
    }


    //process created data and apply meshes and materials
    /*
    updateMesh(grassLayer, true, layerByMaterialEnum(materialEnum::grassMaterial));
    updateMesh(stoneLayer, true, layerByMaterialEnum(materialEnum::stoneMaterial));

    ApplyMaterial(materialEnum::grassMaterial);
    ApplyMaterial(materialEnum::stoneMaterial);*/
    //updateLodLevelAndReloadMesh(ELod::lodNear);

    grassLayer.calculateNormals();
    stoneLayer.calculateNormals();
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




void AcustomMeshActorBase::updateLodLevelAndReloadMesh(ELod level){
    if(level != currentLodLevel){
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
                bool createNormals, 
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


//TESTED
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
    meshcomponent.ClearMeshSection(layer);
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
    AActorUtil::showActor(*this, true);
    AActorUtil::enableColliderOnActor(*this, true);


    if(!enableCollision){
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
        materialEnum::treeMaterial,
        materialEnum::palmLeafMaterial
    };
    return types;
}








/**
 * 
 * 
 * --- lod helper methods ---
 * 
 * 
 */
std::vector<ELod> AcustomMeshActorBase::lodVector(){
    std::vector<ELod> types = {
        ELod::lodFar,
        ELod::lodMiddle,
        ELod::lodNear
    };
    return types;
}