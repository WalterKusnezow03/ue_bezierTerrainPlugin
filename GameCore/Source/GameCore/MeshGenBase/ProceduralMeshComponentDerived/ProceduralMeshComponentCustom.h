#pragma once

#include "CoreMinimal.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"
#include "ProceduralMeshComponentCustom.generated.h"

/// @brief doesnt help.
UCLASS()
class GAMECORE_API UProceduralMeshComponentCustom : public UProceduralMeshComponent {

    GENERATED_BODY()

public:
    void BuildCollisionExternal(TArray<uint8> &bytes);

    void UpdateMesh(int32 sectionIndex, MeshData &dataIn, bool bCreateCollision);

private:
    
};
