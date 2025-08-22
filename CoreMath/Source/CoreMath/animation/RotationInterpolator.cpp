#include "RotationInterpolator.h"

RotationInterpolator::RotationInterpolator(){

}

RotationInterpolator::~RotationInterpolator(){

}

void RotationInterpolator::setTarget(FRotator &a, FRotator &b, float time){
    timeIntegrated = 0.0f;
    start = a;
    end = b;
    overrideTime(time);
}

void RotationInterpolator::overrideTime(float timein){
    interpolationTime = std::abs(timein);
    timeIntegrated = 0.0f;
}

bool RotationInterpolator::endReached(){
    return skalarCurrent() >= 1.0f;
}

FRotator RotationInterpolator::interpolate(float deltatime){
    timeIntegrated += deltatime;
    return interpolationRotation(start, end, skalarCurrent());
}

float RotationInterpolator::skalarCurrent(){
    if(interpolationTime > 0.0001f){
        return timeIntegrated / interpolationTime; //skalar = distTarget / distAll
    }
    return 1.0f;
}

FRotator RotationInterpolator::interpolationRotation(FRotator &fromIn, FRotator &toIn, float skalar){
    if (skalar >= 1.0f)
    {
        return toIn;
    }
    
    // Convert to quaternions
    FQuat fromQuat = fromIn.Quaternion();
    FQuat toQuat = toIn.Quaternion();

    // Use spherical linear interpolation (SLERP)
    FQuat resultQuat = FQuat::Slerp(fromQuat, toQuat, skalar);

    // Normalize to ensure stability
    resultQuat.Normalize();

    // Convert back to Rotator if needed
    return resultQuat.Rotator();
    
}