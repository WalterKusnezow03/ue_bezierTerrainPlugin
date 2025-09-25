#pragma once

#include "CoreMinimal.h"

/// @brief Marked for removal, still might be needed.
class COREMATH_API RotationInterpolator {

public:
    RotationInterpolator();
    ~RotationInterpolator();

    void setTarget(FRotator &a, FRotator &b, float time);

    FRotator interpolate(float deltaTime);

    void overrideTime(float timeIn);

    bool endReached();

private:
    FRotator interpolationRotation(FRotator &fromIn, FRotator &toIn, float skalar);

    float skalarCurrent();

    float timeIntegrated = 0.0f;
    float interpolationTime = 0.0f;

    FRotator start;
    FRotator end;
    
};