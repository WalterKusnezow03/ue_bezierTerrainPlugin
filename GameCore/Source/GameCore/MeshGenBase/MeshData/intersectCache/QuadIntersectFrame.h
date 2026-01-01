#pragma once

#include "TriangleIntersectFrame.h"

class GAMECORE_API FQuadIntersectFrame : public FTriangleIntersectFrame {

public:
    FQuadIntersectFrame();
    ~FQuadIntersectFrame();
    FQuadIntersectFrame(const FQuadIntersectFrame &frame);
    FQuadIntersectFrame &operator=(const FQuadIntersectFrame &frame);

    void Setup(
        const FVector &v0,
        const FVector &v1,
        const FVector &v2,
        const FVector &v3
    );

    virtual bool DoesIntersect(
        const FVector &start,
        const FVector &dir,
        FVector &outIntersectionPoint
    ) override;

protected:
    virtual void CalculateNormals() override;
    FVector v3Projected;
    FVector v3Normal;
};