#include "MeshDataMap.h"

MeshDataMap::MeshDataMap(){

}

MeshDataMap::~MeshDataMap(){

}



MeshData &MeshDataMap::meshDataReference(materialEnum type, bool raycast){
    if(raycast){
        return meshDataReferenceRaycast(type);
    }
    return meshDataReferenceNoRaycast(type);
}

MeshData &MeshDataMap::meshDataReferenceRaycast(materialEnum material){
    if(raycastMeshData.find(material) == raycastMeshData.end()){
        MeshData empty;
        raycastMeshData[material] = empty;
    }
    return raycastMeshData[material];
}

MeshData &MeshDataMap::meshDataReferenceNoRaycast(materialEnum material){
    if(noRaycastMeshData.find(material) == noRaycastMeshData.end()){
        MeshData empty;
        noRaycastMeshData[material] = empty;
    }
    return noRaycastMeshData[material];
}


void MeshDataMap::replaceMeshDataRaycast(MeshData &data, materialEnum type){
    raycastMeshData[type] = data;
}

void MeshDataMap::replaceMeshDataNoRaycast(MeshData &data, materialEnum type){
    noRaycastMeshData[type] = data;
}



/**
 * append mesh data
 */
void MeshDataMap::appendMeshDataRaycast(
    MeshData &meshdata,
    materialEnum type
){
    MeshData &found = meshDataReferenceRaycast(type);
    found.append(meshdata);
}

void MeshDataMap::appendMeshDataNoRaycast(
    MeshData &meshdata,
    materialEnum type
){
    MeshData &found = meshDataReferenceNoRaycast(type);
    found.append(meshdata);
}





//ray intersect checker
bool MeshDataMap::RayIntersectFirstHit(
    const FVector &origin,
    const FVector &direction,
    FVector &outIntersectionPoint
){
    for (auto& pair : raycastMeshData){
        MeshData &data = pair.second;
        if(data.RayIntersect(origin, direction, outIntersectionPoint)){
            return true;
        }
    }
    return false;
}


bool MeshDataMap::RayIntersectClosestHit(
    const FVector &origin,
    const FVector &direction,
    FVector &outIntersectionPoint
){
    TArray<FVector> Hitpoints;
    for (auto& pair : raycastMeshData)
    {
        MeshData &data = pair.second;
        FVector hitTracked;
        if (data.RayIntersect(origin, direction, hitTracked))
        {
            Hitpoints.Add(hitTracked);
        }
    }

    //o(n)
    if(Hitpoints.Num() > 0){
        float distSquaredClosest = FVector::DistSquared(origin, Hitpoints[0]);
        int index = 0;
        for (int i = 1; i < Hitpoints.Num(); i++){
            FVector &current = Hitpoints[i];
            if (float f = FVector::DistSquared(Hitpoints[i], origin); f < distSquaredClosest){
                distSquaredClosest = f;
                index = i;
            }
        }
        
        //index valid in any case (safety check)
        if(index >= 0 && index < Hitpoints.Num()){
            outIntersectionPoint = Hitpoints[index];
            return true;
        }
    }

    return false;
}


bool MeshDataMap::RayIntersect(
    const FVector &origin,
    const FVector &direction
){
    FVector none;
    for (auto& pair : raycastMeshData){
        MeshData &data = pair.second;
        if(data.RayIntersect(origin, direction, none)){
            return true;
        }
    }
    return false;
}