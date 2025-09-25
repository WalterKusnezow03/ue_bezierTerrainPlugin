#pragma once 

#include "CoreMinimal.h"

///will apply the graham scan n times, until no nodes are removed,
///if outline found: remove from set.
///the result is the most inner polygon.
///if the inner nodes count < 3: the whole space is left. 2 is not a polygon.
class COREMATH_API InnerHullFinder{

public:
    InnerHullFinder();
    ~InnerHullFinder();

    void MakeMostInnerHull(TArray<FVector2D> &shape);

    void EliminateOuterHull(TArray<FVector2D> &output);
    void EliminateOuterHull(TArray<FVector2D> &output, int iterations);

private:
};