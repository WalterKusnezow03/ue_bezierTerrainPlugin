#pragma once

#include "CoreMinimal.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"


/// @brief will store a uprocedural mesh with raycast and without including their mesh
/// data for all layers
class GAMECORE_API ProceduralMeshComponentPair{

public:
    ProceduralMeshComponentPair();
    ~ProceduralMeshComponentPair();

    void init(
        FString name,
        int index,
        AActor *actorOwner,
        USceneComponent *RootComponent
    );

    void overrideMeshPair(UProceduralMeshComponent *raycastMeshIn, UProceduralMeshComponent *noraycastIn);

    UProceduralMeshComponent *RaycastMesh();
    UProceduralMeshComponent *noRaycastMesh();

    MeshData &meshDataReference(materialEnum type, bool raycast);
    MeshData &meshDataReferenceRaycast(materialEnum material);
    MeshData &meshDataReferenceNoRaycast(materialEnum material);

    void setCollisionEnabled(bool flag);
    void setHiddenInGame(bool flag);

    void updateMeshAllAndApplyMaterial(materialEnum type);

    void updateMeshAll(materialEnum type);
    void updateMeshRaycast(materialEnum type);
    void updateMeshNoRaycast(materialEnum type);

    void refreshMeshRaycast(materialEnum type);
    void refreshMeshNoRaycast(materialEnum type);

    void replaceMeshDataRaycastAndUpdate(MeshData &data, materialEnum type);
    void replaceMeshDataNoRaycastAndUpdate(MeshData &data, materialEnum type);

    static int layerByMaterialEnum(materialEnum type);

    void ApplyMaterialAll(materialEnum type);
    void ApplyMaterialRaycast(materialEnum type);
    void ApplyMaterialNoRaycast(materialEnum type);

    void appendMeshDataAndReloadRaycast(MeshData &datain, materialEnum type);
    void appendMeshDataAndReloadNoRaycast(
        MeshData &meshdata,
        materialEnum type);

private:
    std::map<materialEnum, MeshData> raycastMeshData;
    std::map<materialEnum, MeshData> noRaycastMeshData; //no physics mesh at all

    UProceduralMeshComponent *raycastMesh = nullptr;
    UProceduralMeshComponent *noraycastMesh = nullptr;

    void updateMesh(
        UProceduralMeshComponent &meshcomponent,
        MeshData &otherMesh, // MUST BE SAVED IN A VALUE CLASS SCOPE SOMEWHERE!
        int layer
    );

    void refreshMesh(
        UProceduralMeshComponent &meshComponent,
        MeshData &other,
        int layer
    );

    void ApplyMaterial(UProceduralMeshComponent *ProceduralMeshComponent, UMaterialInterface *Material);
	void ApplyMaterial(
		UProceduralMeshComponent *ProceduralMeshComponent,
		UMaterialInterface *Material,
		int layer
	);
	void ApplyMaterial(
		UProceduralMeshComponent *ProceduralMeshComponent,
		materialEnum type
	);
};