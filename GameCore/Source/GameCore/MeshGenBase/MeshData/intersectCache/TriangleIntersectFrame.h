#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"


class GAMECORE_API FTriangleIntersectFrame {

public:
    FTriangleIntersectFrame();
    virtual ~FTriangleIntersectFrame();
    FTriangleIntersectFrame(const FTriangleIntersectFrame &frame);
    FTriangleIntersectFrame &operator=(const FTriangleIntersectFrame &frame);

    //optional for meshdata
    void SetIdentifier(int32 v0, int32 v1, int32 v2);
    bool IsSameIdentifier(int32 v0, int32 v1, int32 v2);
    bool HasIdentifier(int32 v0);
    void IncreaseIdentifierBy(int32 offset);

    void CopyIdentifier(int32 &v0, int32 &v1, int32 &v2);

    void Setup(
        const FVector &v0,
        const FVector &v1,
        const FVector &v2
    );


    virtual bool DoesIntersect(
        const FVector &start,
        const FVector &dir,
        FVector &outIntersectionPoint
    );

protected:
    virtual void CalculateNormals();
    FVector CalculateNormal(FVector &v0, FVector &v1);

    FPlane plane;
    MMatrix rotationInverse;

    FVector v0Projected;
    FVector v1Projected;
    FVector v2Projected;

    FVector v0Normal;
    FVector v1Normal;
    FVector v2Normal;

    int32 v0Index = 0;
    int32 v1Index = 0;
    int32 v2Index = 0;
};