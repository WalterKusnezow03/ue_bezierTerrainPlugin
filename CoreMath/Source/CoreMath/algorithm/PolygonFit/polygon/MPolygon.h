#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/2D/MMatrix2D.h"
#include "CoreMath/vector/bound/FBoundingBox2D.h"

class COREMATH_API MPolygon {

public:
    FVector2D locationCopy;

    MPolygon();
    ~MPolygon();

    MPolygon(const MPolygon &other);
    MPolygon &operator=(const MPolygon &other);

    ///@brief recommended to start at 0,0, rotated around this point.
    void SetShape(const TArray<FVector2D> &shapeIn);

    ///@brief recommended to start at 0,0, rotated around this point, use this 
    /// to force a detail on the polygon!
    void SetShape(const TArray<FVector2D> &shapeIn, int minDistanceBetweenPoints);
    void SetShapeTransformed(const TArray<FVector2D> &shapeIn, int minDistanceBetweenPoints);

    TArray<FVector2D> facingParentCenter(FVector2D &centerParent);

protected:
    TArray<FVector2D> IncreaseDetail(const TArray<FVector2D> &shapeIn, int minDistanceBetweenPoints);

public:
    bool DoesIntersectIncludingBoundsCheck(const MPolygon &shapeIn);
    virtual bool DoesIntersect(const MPolygon &shapeIn);
    bool DoesIntersectClockwiseShape(const TArray<FVector2D> &shapeIn);

    //checks hittest alogn "transformed" shape
    bool DoesIntersect(const FVector2D &aWorld, const FVector2D &bWorld);

    bool DoesIntersect(
        const FVector2D &aWorld, 
        const FVector2D &bWorld,
        TArray<FVector2D> &intersectionsOut //is Cleared
    );


    /// modify transform immidiate to check for hit
    /// removes all older transformations
    void SetTransform(const MMatrix2D &matrix);



    /// @brief recreates the shapeTransformed buffer for intersection tests.
    /// called automatically on transformation update and shape override (SetShape)
    void UpdateTransformedShape();

    void CopyShapeTransformed(TArray<FVector2D> &appendTo);

    float BoundingArea(){
        return areaFound;
    }

    TArray<FVector2D> &internalTransformed(){
        return shapeTransformed;
    }

    ///performs a bounds check on this tranformed and other transformed buffer
    bool IsInBound(const MPolygon &polygon);


    //new
    void AppendClosestTo(FVector2D &center, TArray<FVector2D> &updatedShape);


    //new
    void AppendRightSideBounds(TArray<FVector2D> &appendTo);

    int VertexCount(){
        return shapeTransformed.Num();
    }

    ///@brief returns bounding box of transformed vertecies
    const FBoundingBox2D &boundingBox() const;

    ///@brief returns local bounds transformed, logicl of tl, br kept.
    FBoundingBox2D localBoundsTransformed();


    FString ToString();

protected:
    float areaFound = 0.0f;
    void CalculateArea();

    FBoundingBox2D localBounds;
    FBoundingBox2D myBounds;
    void UpdateBounds();

    MMatrix2D myTransform;

    TArray<FVector2D> shapeTransformed;
    TArray<FVector2D> shapeOriginal;

    FVector2D moveAwayFromIntersection;

    bool DoesIntersect(
        const FVector2D &aWorld, // edge 1
        const FVector2D &bWorld,
        const FVector2D &e1, // edge 2
        const FVector2D &e2
    );

    bool DoesIntersect(
        const FVector2D &aWorld, // edge 1
        const FVector2D &bWorld,
        const FVector2D &e1, // edge 2
        const FVector2D &e2,
        FVector2D &outIntersect
    );

    FVector2D EdgeNormal(const FVector2D &connect);
    FVector2D EdgeNormal(const FVector2D &v0, const FVector2D &v1);

    bool SegmentIntersection2D(
        const FVector2D &a0,
        const FVector2D &a1,
        const FVector2D &b0,
        const FVector2D &b1,
        FVector2D &outIntersection
    );

    ///@brief direct hittesting between two points and a target
    bool IntersectsPoint(
        const FVector2D &a0,
        const FVector2D &a1,
        const FVector2D &b
    );

    FVector2D centerOfTransformedShape();



    

};