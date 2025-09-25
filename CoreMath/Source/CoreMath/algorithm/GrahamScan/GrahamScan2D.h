#pragma once

#include "CoreMinimal.h"

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


private:
    bool IsClockwise(const FVector2D &a, const FVector2D &b, const FVector2D &c);
    void ComputeConvexHull(
        TArray<FVector2D> &points,
        TArray<FVector2D> &eliminated,
        bool findEliminated
    );
};