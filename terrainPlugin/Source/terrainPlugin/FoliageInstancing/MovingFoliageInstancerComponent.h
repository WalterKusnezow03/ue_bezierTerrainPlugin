#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameCore/MeshGenBase/ELod.h"
#include "MovingFoliageInstancerComponent.generated.h"

/// @brief attach this component to ameshactor
UCLASS()
class TERRAINPLUGIN_API UMovingFoliageInstancerComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    ///@brief creates a fixed amount of childs which cant be changed.
    void Init(int countChilds, UStaticMesh *someMesh, AActor *parent);
    
    /// @brief updates all instances with the given positions
    /// @param positions 
    void Update(
        const TArray<FVector> &positions,
        const TArray<FVector> &normals,
        ELod lodLevelcurrent
    );

    /// @brief moves all instances away and hides this component
    void HideAll();

protected:
    AActor *parentPtr = nullptr;

    /// @brief updates the transform array with the given position until reached max size.
    /// or remove from visibility
    /// @param positions 
    void UpdateTransformArray(
        const TArray<FVector> &positions,
        const TArray<FVector> &normals
    );

    UPROPERTY()
    UInstancedStaticMeshComponent *instancer = nullptr;

    void SetHiddenInGame(bool flag);

    bool bWorldSpaceFlag = false;
    TArray<FTransform> transformChilds;

    FQuat RotataionForSurfaceNormal(const FVector &normal);
};


