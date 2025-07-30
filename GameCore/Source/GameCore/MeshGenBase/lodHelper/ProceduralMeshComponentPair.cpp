#include "ProceduralMeshComponentPair.h"
#include "GameCore/MeshGenBase/materialHelper/MaterialEnumHelper.h"


ProceduralMeshComponentPair::ProceduralMeshComponentPair(){

}
ProceduralMeshComponentPair::~ProceduralMeshComponentPair(){

}

void ProceduralMeshComponentPair::init(
    FString name, 
    int index,
    AActor *actorOwner,
    USceneComponent *RootComponent
){
    if(RootComponent && actorOwner){
        FString nameMesh = name + FString::Printf(TEXT("raycastMesh_%d"), index);
        raycastMesh = NewObject<UProceduralMeshComponent>(actorOwner, *nameMesh);
        if(raycastMesh){
            raycastMesh->SetupAttachment(RootComponent);
            raycastMesh->RegisterComponent(); //it is not visible otherwise
        }
        

        FString nameMeshNoRaycast = name + FString::Printf(TEXT("noRaycastMesh_%d"), index);
        noraycastMesh = NewObject<UProceduralMeshComponent>(actorOwner, *nameMeshNoRaycast);
        if(noraycastMesh){
            noraycastMesh->SetupAttachment(RootComponent);
            noraycastMesh->RegisterComponent(); //it is not visible otherwise
        }

        setCollisionEnabled(true); //default
    }
}

void ProceduralMeshComponentPair::overrideMeshPair(
    UProceduralMeshComponent *raycastMeshIn, 
    UProceduralMeshComponent *noraycastIn
){
    noraycastMesh = noraycastIn;
    raycastMesh = raycastMeshIn;
}

/// @brief collision enabled on raycast mesh or not.
/// @param flag 
void ProceduralMeshComponentPair::setCollisionEnabled(bool flag){
    if(noraycastMesh){
        noraycastMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    if(raycastMesh){

        ECollisionEnabled::Type collision = flag ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision;
        raycastMesh->SetCollisionEnabled(collision);
    }
}

void ProceduralMeshComponentPair::setHiddenInGame(bool flag){
    if(noraycastMesh){
        noraycastMesh->SetHiddenInGame(flag);
    }
    if(raycastMesh){
        raycastMesh->SetHiddenInGame(flag);
    }
}





UProceduralMeshComponent *ProceduralMeshComponentPair::RaycastMesh(){
    return raycastMesh;
}

UProceduralMeshComponent *ProceduralMeshComponentPair::noRaycastMesh(){
    return noraycastMesh;
}


MeshData &ProceduralMeshComponentPair::meshDataReference(materialEnum type, bool raycast){
    if(raycast){
        return meshDataReferenceRaycast(type);
    }
    return meshDataReferenceNoRaycast(type);
}

MeshData &ProceduralMeshComponentPair::meshDataReferenceRaycast(materialEnum material){
    if(raycastMeshData.find(material) == raycastMeshData.end()){
        MeshData empty;
        raycastMeshData[material] = empty;
    }
    return raycastMeshData[material];
}

MeshData &ProceduralMeshComponentPair::meshDataReferenceNoRaycast(materialEnum material){
    if(noRaycastMeshData.find(material) == noRaycastMeshData.end()){
        MeshData empty;
        noRaycastMeshData[material] = empty;
    }
    return noRaycastMeshData[material];
}






/**
 * 
 * Update !
 * 
 */
void ProceduralMeshComponentPair::updateMeshAll(materialEnum type){
    updateMeshRaycast(type);
    updateMeshNoRaycast(type);
}

void ProceduralMeshComponentPair::updateMeshRaycast(materialEnum type){
    int layer = layerByMaterialEnum(type);
    if(raycastMesh){
        updateMesh(
            *raycastMesh,
            meshDataReferenceRaycast(type),
            layer
        );
    }
}

void ProceduralMeshComponentPair::updateMeshNoRaycast(materialEnum type){
    int layer = layerByMaterialEnum(type);
    if(noraycastMesh){
        updateMesh(
            *noraycastMesh,
            meshDataReferenceNoRaycast(type),
            layer
        );
    }
}



/// @brief replaces the mesh layer for an mesh component
/// caution: mesh section is recreated because modifying the triangle buffer is not allowed
/// when will to update an mesh!
/// @param meshcomponent 
/// @param otherMesh 
/// @param layer 
/// @param enableCollision 
void ProceduralMeshComponentPair::updateMesh(
    UProceduralMeshComponent &meshcomponent,
    MeshData &otherMesh, //MUST BE SAVED IN A VALUE CLASS SCOPE SOMEWHERE!
    int layer
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

}

void ProceduralMeshComponentPair::replaceMeshDataRaycastAndUpdate(MeshData &data, materialEnum type){
    raycastMeshData[type] = data;
    updateMeshRaycast(type);
}

void ProceduralMeshComponentPair::replaceMeshDataNoRaycastAndUpdate(MeshData &data, materialEnum type){
    noRaycastMeshData[type] = data;
    updateMeshNoRaycast(type);
}




/**
 * 
 * refresh mesh
 * 
 */
void ProceduralMeshComponentPair::refreshMeshRaycast(materialEnum type){
    int layer = layerByMaterialEnum(type);
    if(raycastMesh){
        refreshMesh(
            *raycastMesh,
            meshDataReferenceRaycast(type),
            layer
        );
    }
}

void ProceduralMeshComponentPair::refreshMeshNoRaycast(materialEnum type){
    int layer = layerByMaterialEnum(type);
    if(noraycastMesh){
        refreshMesh(
            *noraycastMesh,
            meshDataReferenceNoRaycast(type),
            layer
        );
    }
}



void ProceduralMeshComponentPair::refreshMesh(
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

}







/// @brief returns the layer by material enum type
/// @param type type of material
/// @return int layer index
int ProceduralMeshComponentPair::layerByMaterialEnum(materialEnum type){
    std::vector<materialEnum> types = MaterialEnumHelper::materialVector();
    for (int i = 0; i < types.size(); i++)
    {
        if(type == types[i]){
            return i;
        }
    }
    return 0;
}




/**
 * 
 * Apply materials
 * 
 */
void ProceduralMeshComponentPair::updateMeshAllAndApplyMaterial(materialEnum type){
    updateMeshAll(type);
    ApplyMaterialAll(type);
}

void ProceduralMeshComponentPair::ApplyMaterialAll(materialEnum type){
    ApplyMaterialRaycast(type);
    ApplyMaterialNoRaycast(type);
}

void ProceduralMeshComponentPair::ApplyMaterialRaycast(materialEnum type){
    ApplyMaterial(raycastMesh, type);
}

void ProceduralMeshComponentPair::ApplyMaterialNoRaycast(materialEnum type){
    ApplyMaterial(noraycastMesh, type);
}


/// @brief applys a material to the whole component (slot 0 by default)
/// @param ProceduralMeshComponent 
/// @param Material 
void ProceduralMeshComponentPair::ApplyMaterial(
    UProceduralMeshComponent* ProceduralMeshComponent, 
    UMaterialInterface* Material
) {
    ApplyMaterial(ProceduralMeshComponent, Material, 0);
}

/// @brief applys a material to the whole component at passed index slot
/// @param ProceduralMeshComponent mesh to apply for
/// @param Material material to set
/// @param layer layer to apply for / index
void ProceduralMeshComponentPair::ApplyMaterial(
    UProceduralMeshComponent* ProceduralMeshComponent, 
    UMaterialInterface* Material,
    int layer
) {
	if (ProceduralMeshComponent != nullptr && Material != nullptr) {
		// Apply the material to the first material slot (index 0) of the procedural mesh
		ProceduralMeshComponent->SetMaterial(layer, Material);
    }
}


/// @brief applies material by enum to the found layer on the passed mesh
/// @param ProceduralMeshComponent 
/// @param type 
void ProceduralMeshComponentPair::ApplyMaterial(
    UProceduralMeshComponent* ProceduralMeshComponent,
    materialEnum type
){
    int layer = layerByMaterialEnum(type);
    if (assetManager *e = assetManager::instance())
    {
        ApplyMaterial(ProceduralMeshComponent, e->findMaterial(type), layer);
    }
}



/**
 * append mesh data
 */
void ProceduralMeshComponentPair::appendMeshDataAndReloadRaycast(
    MeshData &meshdata,
    materialEnum type
){
    MeshData &found = meshDataReferenceRaycast(type);
    found.append(meshdata);
    updateMeshRaycast(type);
}

void ProceduralMeshComponentPair::appendMeshDataAndReloadNoRaycast(
    MeshData &meshdata,
    materialEnum type
){
    MeshData &found = meshDataReferenceNoRaycast(type);
    found.append(meshdata);
    updateMeshNoRaycast(type);
}


