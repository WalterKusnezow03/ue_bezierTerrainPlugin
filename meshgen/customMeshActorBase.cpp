// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "p2/util/FVectorTouple.h"
#include "p2/util/AActorUtil.h"
#include "p2/pathFinding/PathFinder.h"
#include "Components/BoxComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "p2/meshgen/generation/bezierCurve.h"
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
    MeshData grassLayer;
    MeshData stoneLayer;


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
                        //buildQuad(vzero, vone, vtwo, vthree, output_grass_layer, triangles_grass_layer);

                        createQuad(vzero, vone, vtwo, vthree, grassLayer);
                    }
                    else
                    {
                        //otherwise the quad should be added to the second
                        //triangle / vertecy array for stone material, more vertical
                        //buildQuad(vzero, vone, vtwo, vthree, output_stone_layer, triangles_stone_layer);

                        createQuad(vzero, vone, vtwo, vthree, stoneLayer);
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
    updateMesh(grassLayer, true, layerByMaterialEnum(materialEnum::grassMaterial));
    updateMesh(stoneLayer, true, layerByMaterialEnum(materialEnum::stoneMaterial));

    ApplyMaterial(materialEnum::grassMaterial);
    ApplyMaterial(materialEnum::stoneMaterial);


    
	
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






/// @brief updates a mesh layer given on a mesh data object (which will be deep copied)
/// @param otherMesh some mesh, will be COPIED
/// @param createNormals recreate normals
/// @param layer layer to save in
void AcustomMeshActorBase::updateMesh(MeshData &otherMesh, bool createNormals, int layer){
    if(Mesh){
        updateMesh(
            *Mesh,
            otherMesh,
            createNormals,
            layer,
            meshLayersMap,
            true // enableCollision
        );
    }
    /*
    meshLayersMap[layer] = otherMesh; //assign operator is overriden

    MeshData *data = nullptr;
    if (meshLayersMap.find(layer) != meshLayersMap.end()){
        //find meshData from map by reference
        data = &meshLayersMap[layer]; //hier mit eckigen klammern weil .find ein iterator ist
    }

    if(data != nullptr && Mesh != nullptr){
        
        
        if(createNormals){
            data->calculateNormals();
        }

        / *
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
        );* / 
        Mesh->ClearMeshSection(layer);
        Mesh->CreateMeshSection(
            layer, 
            data->getVerteciesRef(),//newvertecies, 
            data->getTrianglesRef(),//this->triangles, 
            data->getNormalsRef(),//normals, 
            data->getUV0Ref(),//UV0, 
            data->getVertexColorsRef(),//VertexColors, 
            data->getTangentsRef(),//Tangents, 
            true
        );

        //set for spehere overlap
        Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
        Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

    }
    //enable if was disabled!
    AActorUtil::showActor(*this, true);
    AActorUtil::enableColliderOnActor(*this, true);
    */

}



void AcustomMeshActorBase::updateMeshNoRaycastLayer(MeshData &otherMesh, bool createNormals, int layer){
    if(MeshNoRaycast){
        updateMesh(
            *MeshNoRaycast,
            otherMesh,
            createNormals,
            layer,
            meshLayersMapNoRaycast,
            false // enableCollision
        );
    }
}


void AcustomMeshActorBase::updateMesh(
    UProceduralMeshComponent &meshcomponent,
    MeshData &otherMesh, 
    bool createNormals, 
    int layer, 
    std::map<int, MeshData> &map,
    bool enableCollision
){
    map[layer] = otherMesh; //assign operator is overriden

    MeshData *data = nullptr;
    if (map.find(layer) != map.end()){
        //find meshData from map by reference
        data = &map[layer]; //hier mit eckigen klammern weil .find ein iterator ist
    }

    if(data != nullptr){
        
        
        if(createNormals){
            data->calculateNormals();
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
            data->getVerteciesRef(),//newvertecies, 
            data->getTrianglesRef(),//this->triangles, 
            data->getNormalsRef(),//normals, 
            data->getUV0Ref(),//UV0, 
            data->getVertexColorsRef(),//VertexColors, 
            data->getTangentsRef(),//Tangents, 
            true
        );

        //set for spehere overlap
        meshcomponent.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        meshcomponent.SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
        meshcomponent.SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

    }
    //enable if was disabled!
    AActorUtil::showActor(*this, true);
    AActorUtil::enableColliderOnActor(*this, true);


    if(!enableCollision){
        MeshNoRaycast->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }else{
        MeshNoRaycast->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }
}
















/// @brief all quads MUST BE BUILD out of TRIANGLES, OTHERWISE MANY BUGS OCCUR!
/// @param a corner 0
/// @param b corner 1
/// @param c corner 2
/// @param output output to append in
/// @param trianglesOutput triangle int32 as nums saved in here, also appended
void AcustomMeshActorBase::buildTriangle(
    FVector &a, 
    FVector &b, 
    FVector &c,
    TArray<FVector> &output,
    TArray<int32> &trianglesOutput
){
    //add vertecies
    output.Add(a);
    output.Add(b);
    output.Add(c);

    //add triangles
    int32 offset = trianglesOutput.Num();

    trianglesOutput.Add(0 + offset); // 0th vertex in the first triangle
    trianglesOutput.Add(1 + offset); // 1st vertex in the first triangle
    trianglesOutput.Add(2 + offset); // 2nd vertex in the first triangle
    
}



/// @brief build a quad out of two triangles! Important otherwise unfixable issues are in the mesh
/// @param a 
/// @param b 
/// @param c 
/// @param d 
/// @param output 
/// @param trianglesOutput 
void AcustomMeshActorBase::buildQuad(
    FVector &a, 
    FVector &b, 
    FVector &c, 
    FVector &d, 
    TArray<FVector> &output,
    TArray<int32> &trianglesOutput
){

    //must be individual triangles:
    //quads: buggy + the engine is converting it to triangles back again anyway
    buildTriangle(a, b, c, output, trianglesOutput);
    buildTriangle(a, c, d, output, trianglesOutput);
    return;

    /*
                1--2
                |  |
                0<-3

                b--c
                |  |
                a<-d
    */

}





void AcustomMeshActorBase::createQuad(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		MeshData &output
){
    output.append(a,b,c,d);
}





void AcustomMeshActorBase::createTwoSidedQuad(
    FVector &a,
	FVector &b,
	FVector &c,
	FVector &d,
	MeshData &output
){
    createQuad(a, b, c, d, output);
    createQuad(a, d, c, b, output);
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
    UMaterial *material
){
    createTwoSidedQuad(a, b, c, d, material, false);
}

void AcustomMeshActorBase::createTwoSidedQuad(
    FVector &a, 
    FVector &b,
    FVector &c,
    FVector &d,
    UMaterial *material,
    bool calculateNormals
){
    if(material != nullptr){
        MeshData meshDataOut;
        createTwoSidedQuad(a, b, c, d, meshDataOut);

        updateMesh(meshDataOut, calculateNormals, 0);
        ApplyMaterial(Mesh, material, 0);

        DebugHelper::logMessage("material was not null!");
    }
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
    std::vector<materialEnum> types = {
        materialEnum::grassMaterial,
        materialEnum::wallMaterial,
        materialEnum::glassMaterial,
        materialEnum::stoneMaterial,
        materialEnum::sandMaterial,
        materialEnum::treeMaterial,
        materialEnum::palmLeafMaterial
    };
    for (int i = 0; i < types.size(); i++){
        if(type == types[i]){
            return i;
        }
    }
    return 0;
}


