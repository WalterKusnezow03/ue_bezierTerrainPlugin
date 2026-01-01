#pragma once

#include "CoreMinimal.h"
#include "CoreMath/algorithm/GrahamScan/ElementWrapper/TIndexedElement.h"

template class TIndexedElement<FVector2D>;

/// @brief clock wise convex hull maker 2d
class COREMATH_API GrahamScan2D {

public:
    GrahamScan2D();
    ~GrahamScan2D();

    ///@brief makes a convex hull from the given points, array modifed
    void ComputeConvexHull(TArray<FVector2D> &points);

    ///@brief makes a convex hull from the given points, array modifed, but also tracks the
    ///eliminated inner points
    void ComputeConvexHull(TArray<FVector2D> &points, TArray<FVector2D> &eliminated);


    ///@brief makes a convex hull from the given points, array not modifed but index array out
    ///put modified: indices are made from points given, convex hull.
    void ComputeConvexHull(const TArray<FVector2D> &points, TArray<int> &outIndices);



private:
    bool IsClockwise(const FVector2D &a, const FVector2D &b, const FVector2D &c);
    bool IsClockwise(
        const TIndexedElement<FVector2D> &a, 
        const TIndexedElement<FVector2D> &b, 
        const TIndexedElement<FVector2D> &c
    );
    void ComputeConvexHull(
        TArray<FVector2D> &points,
        TArray<FVector2D> &eliminated,
        bool findEliminated
    );
};