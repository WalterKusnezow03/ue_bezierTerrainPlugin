#pragma once

#include "CoreMinimal.h"


/// @brief bounding box for 2D Slate mesh data
/*
(tl)
0,0------ 1,0
|          |
0,1-------1,1(br)
*/
struct COREMATH_API FBoundingBox2D {

    FBoundingBox2D();
    virtual ~FBoundingBox2D();
    FBoundingBox2D(const FBoundingBox2D & other);
    FBoundingBox2D &operator=(const FBoundingBox2D & other);

    void Reset();

    // basic update
    FBoundingBox2D(const TArray<FVector2D> &buffer);
    void Update(const TArray<FVector2D> &buffer);
    void Update(const FVector2D &pos);
    void Update(const FVector2D &v0, const FVector2D &v1, const FVector2D &v2);

    /// @brief min coordinate on x and y
    FVector2D topLeft;
    /// @brief max coordiate on x and y
    FVector2D bottomRight;

    FVector2D size() const;
    float sizeX() const;
    float sizeY() const;

    ///@brief minX minY 
    FVector2D min() const;

    ///@brief maxX maxY
    FVector2D max() const;

    FVector2D minXmaxY() const;
    FVector2D maxXminY() const;

    FVector2D center() const;

    bool OtherIsInside(const FBoundingBox2D &other);

    FString ToString() const;

    TArray<FVector2D> asVertecies();
};