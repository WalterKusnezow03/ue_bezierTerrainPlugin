#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"

///EDITOR ONLY
///will generate grass mesh data for editor asset.
class TERRAINPLUGIN_API MatrixGrass {

public:
    MatrixGrass();
    ~MatrixGrass();

    MeshData Generate(int sizecm, int density, FVector2D pivotUv = FVector2D(0,0));

private:
    void CreateSingleStraw(MeshData &dataAppend, MMatrix &transform);
    void CreateSprite(MeshData &dataAppend, MMatrix &transform);
};