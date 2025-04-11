#pragma once


#include "CoreMinimal.h"

class P2_API BoundingBox{

public:
    BoundingBox();
    BoundingBox(TArray<FVector> &vertecies);
    ~BoundingBox();

    FVector bottomLeftNearVertex();
    FVector topRightLeftVertex();

    void updateBoundsIfNeeded(TArray<FVector> &vertecies);
    void updateBoundsIfNeeded(FVector &other);

    bool isInsideBoundingbox(FVector &other);

private:
    FVector bottomLeftNear;
    FVector topRightLeft;
};