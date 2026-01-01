#pragma once

#include "CoreMinimal.h"
#include "AssetEnumCollection/assetEnums/materialEnum.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"
#include "GameCore/MeshGenBase/materialHelper/MaterialEnumHelper.h"

struct TERRAINPLUGIN_API FMeshDataIdentifier {

    void Setup(materialEnum typeIn){
        type = typeIn;
        layerNumber = MaterialEnumHelper::indexFor(typeIn);
        name = MaterialEnumHelper::toString(typeIn);
    }

    void Setup(materialEnum typeIn, MeshData *ptrIn, ELod lodIn, bool bRaycast){
        Setup(typeIn);
        meshDataPointer = ptrIn;
        lodLevel = lodIn;
        raycastOnLayer = bRaycast;
    }

    void UpdateMaterialFromName(FString nameIn){
        name = nameIn;
        type = MaterialEnumHelper::toMaterialEnum(name);
    }

    MeshData *meshDataPointer = nullptr;
    FString name;
    materialEnum type = materialEnum::grassMaterial;
    int layerNumber = 0;

    ELod lodLevel = ELod::lodNear;
    bool raycastOnLayer = true;
};