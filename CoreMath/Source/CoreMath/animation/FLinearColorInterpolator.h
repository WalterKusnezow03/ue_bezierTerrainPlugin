#pragma once 

#include "CoreMinimal.h"
#include "CoreMath/animation/InterpolatorBase/TInterpolator.h"

class COREMATH_API FLinearColorInterpolator : public TInterpolator<FLinearColor> {

public:
    FLinearColorInterpolator();
    virtual ~FLinearColorInterpolator();

    virtual FLinearColor interpolation(
        FLinearColor &fromIn, 
        FLinearColor &toIn, 
        float skalarCurrent
    ) override;
    virtual float Distance(FLinearColor &a, FLinearColor &b) override;

private:
    FVector to3D(FLinearColor &color);
};