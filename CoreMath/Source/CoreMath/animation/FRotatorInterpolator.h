#pragma once 
#include "CoreMinimal.h"
#include "CoreMath/animation/InterpolatorBase/TInterpolator.h"

class COREMATH_API FRotatorInterpolator : public TInterpolator<FRotator> {

public:
    bool bUseQuat = true;


    FRotatorInterpolator();
    virtual ~FRotatorInterpolator();

    virtual float Distance(FRotator &a, FRotator &b) override;

    
    virtual FRotator interpolation(FRotator &fromIn, FRotator &toIn, float skalarCurrent) override;

    virtual bool endReached() override {
        return skalar() >= 1.0f || hasReachedTarget(); //kleiner test, hasReachedTarget() added here.
    }

    void SetUsePrimitiveInterpolationTrue(){
        bUseQuat = false;
    }

protected:
    FRotator QuatInterpolation(FRotator &fromIn, FRotator &toIn, float skalarCurrent);
    FRotator PrimitveInterpolation(FRotator &fromIn, FRotator &toIn, float skalarCurrent);

    float shorterAngleSum(FRotator &a, FRotator &b);
    float rotationDirectionShorter(float a, float b);
    float wrapAngle180(float angle);
};