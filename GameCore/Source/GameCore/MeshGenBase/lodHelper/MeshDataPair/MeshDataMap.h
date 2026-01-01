#pragma once

#include "CoreMinimal.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"

class ProceduralMeshComponentPair; // forward declaration

/// @brief base class to store meshdata for raycast enabled and non enabled layers
class GAMECORE_API MeshDataMap {
    //Allow ProceduralMeshComponentPair derived from this, using the protected members of another object.
    friend class ProceduralMeshComponentPair;

public:
    MeshDataMap();
    ~MeshDataMap();

    MeshData &meshDataReference(materialEnum type, bool raycast);
    MeshData &meshDataReferenceRaycast(materialEnum material);
    MeshData &meshDataReferenceNoRaycast(materialEnum material);

    void replaceMeshDataRaycast(MeshData &data, materialEnum type);
    void replaceMeshDataNoRaycast(MeshData &data, materialEnum type);

    void appendMeshDataRaycast(MeshData &datain, materialEnum type);
    void appendMeshDataNoRaycast(
        MeshData &meshdata,
        materialEnum type
    );

    ///@brief will check the whole meshdata map for intersection
    /// (use this from uproeceduralmeshcomponentpair - acustommeshactor)
    /// for raycast alternative
    /// ONLY CHECKS raycastMeshData MAP, Not no raycast!
    /// returns hitpoint with first intersected layer!
    bool RayIntersectFirstHit(
        const FVector &origin,
        const FVector &direction,
        FVector &outIntersectionPoint
    );
    bool RayIntersectClosestHit(
        const FVector &origin,
        const FVector &direction,
        FVector &outIntersectionPoint
    );

    ///checks if any intersection happened.
    bool RayIntersect(
        const FVector &origin,
        const FVector &direction
    );

    ///----> could return all or closest hitpoint too!



protected: 

    std::map<materialEnum, MeshData> raycastMeshData;
    std::map<materialEnum, MeshData> noRaycastMeshData; //no physics mesh at all

    
};