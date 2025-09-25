#pragma once 
#include "CoreMinimal.h"
#include "CoreMath/animation/InterpolatorBase/TInterpolator.h"

class COREMATH_API FVectorInterpolator : public TInterpolator<FVector> {

public:

    FVectorInterpolator(){}
    virtual ~FVectorInterpolator(){}

    virtual FVector interpolate(float DeltaTime) override;

    virtual float Distance(FVector &a, FVector &b) override;

    //must be overriden here.
    virtual FVector interpolation(FVector &fromIn, FVector &toIn, float skalarCurrent) override {
        FVector connect = toIn - fromIn; // AB = B - A
        //gx = A + r (B - A)
        FVector interpolated = fromIn + skalarCurrent * connect;
        return interpolated;
    }

    void setHermiteSplineFlag(bool flag){
        useHermiteSplineFlag = flag;
    }

protected:
    FVector HermiteInterpolate(float skalar);
    FVector HermiteInterpolate(
        FVector &p0, // p0 line start
        FVector &p1, // p1 line end
        FVector &m0, // tangente at p0
        FVector &m1, // tangente at p1
        float t
    );

    bool useHermiteSplineFlag = false;
};