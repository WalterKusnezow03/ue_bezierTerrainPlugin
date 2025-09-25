#pragma once

#include "CoreMinimal.h"
class BoundingBoxSimple;



/// @brief will generate Planar uv coordinates for a normal and buffer deprojecting it on the xy pane.
/// not tested, class i have rewirtten from an external project, no use case here
class GAMECORE_API UVMapper2D{

public:
    UVMapper2D();
    ~UVMapper2D();

    void generateUVBuffer(
        const TArray<FVector> &vertecies,
        FVector normal,
        TArray<FVector2D> &outUV
    );

private:
    FVector2D generateUVCoordinate(
        FVector &vertex,
        BoundingBoxSimple &boundingBox
    );
};