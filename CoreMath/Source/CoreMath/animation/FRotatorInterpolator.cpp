#include "FRotatorInterpolator.h"

FRotatorInterpolator::FRotatorInterpolator(){

}

FRotatorInterpolator::~FRotatorInterpolator(){

}


float FRotatorInterpolator::Distance(FRotator &a, FRotator &b){
    if(bUseQuat){
        //using quat just as interpolation
        FQuat QuatA = a.Quaternion();
        FQuat QuatB = b.Quaternion();
        float AngleDiff = QuatA.AngularDistance(QuatB); // in Radiant
        float DegreesDiff = FMath::RadiansToDegrees(AngleDiff);
        return DegreesDiff;
    }
    //dont know, this is also possible, doesnt matter.
    return shorterAngleSum(a, b);
    
}


FRotator FRotatorInterpolator::interpolation(FRotator &fromIn, FRotator &toIn, float skalarCurrent){
    if (skalarCurrent >= 1.0f)
    {
        return toIn;
    }
    //dont know, this is also possible, doesnt matter. - i find it tricky to understand interpolation
    //of rotation. In theory it could be also a vector from R1 * xAxis to R2 * axis, interpolated and 
    //a look matrix generated. I dont know anymore if i tested that but i sticked to FQuat slerp for a reason.
    //it works, no one cares i guess...
    if(bUseQuat){
        return QuatInterpolation(fromIn, toIn, skalarCurrent);
    }
    return PrimitveInterpolation(fromIn, toIn, skalarCurrent);
}

FRotator FRotatorInterpolator::QuatInterpolation(FRotator &fromIn, FRotator &toIn, float skalarCurrent){
    // Convert to quaternions
    FQuat fromQuat = fromIn.Quaternion();
    FQuat toQuat = toIn.Quaternion();

    // Use spherical linear interpolation (SLERP)
    FQuat resultQuat = FQuat::Slerp(fromQuat, toQuat, skalarCurrent);

    // Normalize to ensure stability
    resultQuat.Normalize();

    // Convert back to Rotator if needed
    return resultQuat.Rotator();
}

FRotator FRotatorInterpolator::PrimitveInterpolation(FRotator &fromIn, FRotator &toIn, float skalarCurrent){
    float deltaRoll = toIn.Roll - fromIn.Roll;
    float deltaPitch = toIn.Pitch - fromIn.Pitch;
    float deltaYaw = toIn.Yaw - fromIn.Yaw;
    FRotator result = fromIn;
    result.Roll += deltaRoll * skalarCurrent;
    result.Pitch += deltaPitch * skalarCurrent;
    result.Yaw += deltaYaw * skalarCurrent;
    return result;
}

float FRotatorInterpolator::wrapAngle180(float angle) {
    angle = std::fmod(angle + 180.0f, 360.0f);
    if (angle < 0.0f)
        angle += 360.0f;
    return angle - 180.0f;
}

float FRotatorInterpolator::rotationDirectionShorter(float a, float b){
    a = wrapAngle180(a);
    b = wrapAngle180(b);

    float diff = b - a;

    if (diff > 180.0f) diff -= 360.0f;
    if (diff < -180.0f) diff += 360.0f;

    return diff;
}




float FRotatorInterpolator::shorterAngleSum(FRotator &a, FRotator &b){
    float sum = 0.0f;
    sum += std::abs(rotationDirectionShorter(a.Roll, b.Roll));
    sum += std::abs(rotationDirectionShorter(a.Pitch, b.Pitch));
    sum += std::abs(rotationDirectionShorter(a.Yaw, b.Yaw));
    return sum;
}