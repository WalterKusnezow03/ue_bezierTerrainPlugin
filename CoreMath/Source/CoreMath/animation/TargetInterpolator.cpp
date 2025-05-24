// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetInterpolator.h"

TargetInterpolator::TargetInterpolator()
{
    reached = true;
    targetSetup = false;
    timeToFrame = 0.0f;
    prev = FVector(0, 0, 0);
}

TargetInterpolator::~TargetInterpolator()
{
}

void TargetInterpolator::useHermiteSplineInterpolation(bool flag){
    useHermiteSplineFlag = flag;
}

bool TargetInterpolator::hasTargetSetup(){
    return targetSetup;
}

void TargetInterpolator::setTarget(FVector fromIn, FVector totarget, float timeToFrameIn){
    setNewTimeToFrame(timeToFrameIn);
    deltaTime = 0.0f;
    reached = false;
    targetSetup = true;
    overrideStart(fromIn);
    overrideTarget(totarget);
    updateReachedFlagBasedOnDistance();
}

void TargetInterpolator::setTarget(
	FVector fromIn, 
	FVector toTarget, 
	FRotator fromRotationIn, 
	FRotator toRotationIn, 
	float timeToFrameIn
){
    setTarget(fromIn, toTarget, timeToFrameIn);
    fromRotation = fromRotationIn;
    toRotation = toRotationIn;
}

void TargetInterpolator::setTarget(
    FRotator fromRotationIn, 
    FRotator toRotationIn, 
    float timeToFrameIn
){
    setNewTimeToFrame(timeToFrameIn);
    reached = false;
    targetSetup = true;
    fromRotation = fromRotationIn;
    toRotation = toRotationIn;
}

/// @brief override target of a RUNNING ANIMATION!
/// @param totarget 
void TargetInterpolator::overrideTarget(FVector totarget){

    //DebugHelper::showScreenMessage("override target, connect:", FColor::Red);
    target = totarget;
}

void TargetInterpolator::overrideStart(FVector fromtarget){
    from = fromtarget;
}

void TargetInterpolator::overrideStart(FRotator fromRotationIn){
    fromRotation = fromRotationIn;
}


void TargetInterpolator::overrideStart(FVector fromtarget, FRotator fromRotationIn){
    overrideStart(fromtarget);
    fromRotation = fromRotationIn;
}
void TargetInterpolator::overrideTarget(FVector totarget, FRotator toRotationIn){
    overrideTarget(totarget);
    toRotation = toRotationIn;
}


void TargetInterpolator::overrideTarget(FRotator toRotationIn){
    toRotation = toRotationIn;
}

void TargetInterpolator::overrideStartSpeedRelative(FVector newStart, FRotator newRoation){
    overrideStartSpeedRelative(newStart);
    fromRotation = newRoation;
}

void TargetInterpolator::overrideStartSpeedRelative(FVector newStart){
    /**
     * was passiert hier:
     * alte distanz und time to frame ergeben velocity
     *
     * velocity: m/s
     * newDist: m
     *
     * newtime = (m) / (m/s) = m * (s/m) = s
     * 
     * metersnew / speed = m * (s/m) = s
     * 
     */

    float distanceOld = FVector::Dist(from, target);
    float speed = distanceOld / timeToFrame; //sei distanz 40m und ttf 2s, dann sinds 20ms


    // Aktualisiere Startpunkt
    overrideStart(newStart);

    // Neue Time-to-Frame berechnen
    float newDistance = FVector::Dist(from, target);
    float newTimeToFrame = newDistance / speed;

    // Neue deltaTime basierend auf dem alten Fortschritt
    timeToFrame = newTimeToFrame;

    if(timeToFrame < 0.01f){
        reached = true;
    }

    if(!reached){
        updateReachedFlagBasedOnDistance();
    }
    
}

void TargetInterpolator::overrideStartSpeedRelative(FRotator newRotation){
    float distanceOld = shorterAngleSum(fromRotation, toRotation);
    float speed = distanceOld / timeToFrame; //sei distanz 40m und ttf 2s, dann sinds 20ms

    // Aktualisiere Startrotation
    fromRotation = newRotation;

    // Neue Time-to-Frame berechnen
    float newDistance = shorterAngleSum(fromRotation, toRotation);
    float newTimeToFrame = newDistance / speed;

    // Neue deltaTime basierend auf dem alten Fortschritt
    timeToFrame = newTimeToFrame;

    if(timeToFrame <= 0.01f){
        reached = true;
    }
}








void TargetInterpolator::resetDeltaTime(){
    deltaTime = 0.0f;
    reached = false;
}

/// @brief sets the time to frame and reached flag is set to false
/// @param time 
void TargetInterpolator::setNewTimeToFrame(float time){
    timeToFrame = std::abs(time);
    reached = false;
}


bool TargetInterpolator::hasReachedTarget(){
    return reached;
}


FVector TargetInterpolator::interpolate(float DeltaTime){
    if(reached){
        worldtargetSetup = false;
        return target;
    }
    deltaTime += DeltaTime;
    float skalarCurrent = skalar();

    if(deltaTime >= timeToFrame || skalarCurrent >= 1.0f){
        reached = true;
        deltaTime = 0.0f;
        worldtargetSetup = false;
        return target;
    }


    
    FVector connect = target - from; // AB = B - A  
    //gx = A + r (B - A)
    //FVector interpolated = from + skalar() * connect;

    
    
    FVector interpolated = TargetInterpolator::interpolation(from, target, skalarCurrent);

    if(useHermiteSplineFlag){
        interpolated = HermiteInterpolate(skalarCurrent); //new debug test
    }


    



    //wenn die richtungs vektoren anti paralell zu einander liegen
    //kann ich prüfen ob mein punkt passiert wurde
    FVector dirToB = target - interpolated;
    float dotProduct = FVector::DotProduct(dirToB.GetSafeNormal(), connect.GetSafeNormal());
    
    //is tested
    //DebugHelper::showScreenMessage("dot product: ", dotProduct);
    /*if (dotProduct < 0.0f)
    {
        DebugHelper::showScreenMessage("PASSED FRAME");
        // anti parellell
        reached = true;
        deltaTime = 0.0f;
        worldtargetSetup = false;
        prev = from;
        return interpolated;
    }*/

   
    return interpolated;

}


FRotator TargetInterpolator::interpolateRotationOnly(float DeltaTime){
    if(deltaTime >= timeToFrame){
        reached = true;
        deltaTime = 0.0f;
        worldtargetSetup = false;
        return toRotation;
    }
    deltaTime += DeltaTime;

    float skalarCurrent = skalar();

    if(skalarCurrent >= 1.0f){
        reached = true;
        deltaTime = 0.0f;
        worldtargetSetup = false;
        return toRotation;
    }
    //DebugHelper::showScreenMessage("skalar rotation", skalarCurrent); //teilweise NAN wert, unendllich

    
    FRotator rotationOutgoing = TargetInterpolator::interpolationRotation(
        fromRotation,
        toRotation,
        skalarCurrent // current skalar
    );

    //ISSUES WITH REACHING, is not switching!!

    return rotationOutgoing;
}


FVector TargetInterpolator::interpolate(float DeltaTime, FRotator &rotationOutgoing){
    FVector outpos = interpolate(DeltaTime);
    
    float skalarCurrent = skalar();
    FRotator interpolatedRotation = TargetInterpolator::interpolationRotation(
        fromRotation, 
        toRotation, 
        skalarCurrent
    );
    rotationOutgoing = interpolatedRotation;
    return outpos;
}

MMatrix TargetInterpolator::interpolateAndGenerateTransform(float DeltaTime){
    MMatrix outMatrix;
    FRotator rotatorOut;
    FVector pos = interpolate(DeltaTime, rotatorOut);
    outMatrix.setRotation(rotatorOut);
    outMatrix.setTranslation(pos);
    return outMatrix;
}

float TargetInterpolator::skalar(){
    //hier muss ein epsilon value sein!
    //1 / 0 = unendlich
    float epsilon = 0.1f;
    if (timeToFrame <= 0.0f + epsilon)
    {
        return 1.0f;
    }

    float skal = deltaTime / timeToFrame; //fractionThis = this / all
    
    if(skal > 1.0f){
        skal = 1.0f;
    }
    if(skal < 0.0f){
        skal = 0.0f;
    }

    // Berechnet den Skalierungsfaktor `t`, der zwischen 0 und 1 liegt
    return skal; //t / 1 quasi.

}



/*

---- static methods -----


*/

FVector TargetInterpolator::interpolation(FVector fromIn, FVector toIn, float skalar){
    FVector connect = toIn - fromIn; // AB = B - A
    //gx = A + r (B - A)
    FVector interpolated = fromIn + skalar * connect;
    return interpolated;
}

FRotator TargetInterpolator::interpolationRotation(FRotator fromIn, FRotator toIn, float skalar){
    if(skalar >= 1.0f){
        return toIn;
    }

    FRotator output;
    output.Roll = fromIn.Roll + skalar * rotationDirectionShorter(fromIn.Roll, toIn.Roll);
    output.Pitch = fromIn.Pitch + skalar * rotationDirectionShorter(fromIn.Pitch, toIn.Pitch);
    output.Yaw = fromIn.Yaw + skalar * rotationDirectionShorter(fromIn.Yaw, toIn.Yaw);

    if(FMath::IsNaN(output.Roll)){
        output.Roll = 0.0f;
    }
    if(FMath::IsNaN(output.Pitch)){
        output.Pitch = 0.0f;
    }
    if(FMath::IsNaN(output.Yaw)){
        output.Yaw = 0.0f;
    }


    return output;
}


float TargetInterpolator::wrapAngle180(float angle) {
    angle = std::fmod(angle + 180.0f, 360.0f);
    if (angle < 0.0f)
        angle += 360.0f;
    return angle - 180.0f;
}

float TargetInterpolator::rotationDirectionShorter(float a, float b){
    a = wrapAngle180(a);
    b = wrapAngle180(b);

    float diff = b - a;

    if (diff > 180.0f) diff -= 360.0f;
    if (diff < -180.0f) diff += 360.0f;

    return diff;
}


/*
/// @brief creates the shorter rotation direction between two angles a and b which can be signed
/// but will be clamped from -180 to 180 degrees
/// @param a 
/// @param b 
/// @return signed shorter angle
float TargetInterpolator::rotationDirectionShorter(float a, float b){
    
    a = std::fmod(a + 180.0f, 360.0f) - 180.0f;
    b = std::fmod(b + 180.0f, 360.0f) - 180.0f;

    float diff = b - a;
    
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    if (diff < -180.0f) {
        diff += 360.0f;
    }

    return diff;


}
*/


float TargetInterpolator::shorterAngleSum(FRotator &a, FRotator &b){
    float sum = 0.0f;
    sum += std::abs(rotationDirectionShorter(a.Roll, b.Roll));
    sum += std::abs(rotationDirectionShorter(a.Pitch, b.Pitch));
    sum += std::abs(rotationDirectionShorter(a.Yaw, b.Yaw));
    return sum;
}

float TargetInterpolator::TimeToFrame(){
    return timeToFrame;
}



FVector TargetInterpolator::readFromPosition(){
    return from;
}

FVector TargetInterpolator::readToPosition(){
    return target;
}








/** 
 * 
 * 
 * ---- WORLD SPACE SECTION ----
 * 
 * 
 * 
 * man könnte hier noch überlegen ob man start local und worldgleichzeitig baut
 * 
 * 
*/


void TargetInterpolator::overrideStartWorld(FVector targetIn){
    fromWorld = targetIn;
}

void TargetInterpolator::overrideTargetWorld(FVector targetIn){
    targetWorld = targetIn;
    worldtargetSetup = true;
}

/// @brief overrides the starting position of the interpolation in world position
/// if the world target is setup, the local target will be also updated, relative to the actor matrix transform
/// @param newStart new world frame start
/// @param actor actor with rotation and translation
void TargetInterpolator::overrideStartWorldSpeedRelative(FVector newStart, MMatrix &actor){

    MMatrix inverse = actor.jordanInverse();
    if (!worldtargetSetup)
    {
        //translate start to local and override start
        FVector startLocal = inverse * newStart;
        overrideStartSpeedRelative(startLocal);
        return;
    }else{
        
        //translate start and target to local and override both, target must be available
        FVector targetLocalNew = inverse * targetWorld;
        overrideTarget(targetLocalNew);
        FVector startLocal = inverse * newStart;
        overrideStartSpeedRelative(startLocal);
        return;
    }
}










/**
 * hermite spline inetrpolation
 */
FVector TargetInterpolator::HermiteInterpolate(float skalar){
    //define tangents for start and end velocity
    FVector tangentA(10, 0, 0); //0,0,100 makes funny yeet up
    FVector tangentB = (target - from).GetSafeNormal() * 10.0f;

    return HermiteInterpolate(from, target, tangentA, tangentB, skalar);
}

FVector TargetInterpolator::HermiteInterpolate(
    FVector &p0, //p0 line start
    FVector &p1, //p1 line end
    FVector &m0, //tangente at p0
    FVector &m1, //tangente at p1
    float t
){
    /**
     *  f(t) = at^3 + bt^2 + ct + d
     * 
     *  hermite spline known formula
     * 	h_0(t) = 2t^3 - 3t^2 + 1
     *  h_1(t) = -2t^3 + 3t^2
	 *  h_2(t) = t^3 - 2t^2 + t
     *  h_3(t) = t^3 - t^2
     * 
     *  hermite(t) = h0 * p0 + h1 * p1 + h2 * tangent1 + h3 * tangent3
     * 
     * 
     */


    float t2 = t * t;
    float t3 = t2 * t;

    float h0 = 2 * t3 - 3 * t2 + 1; //f(0) = 1 --> p0
    float h1 = -2 * t3 + 3 * t2;    //f(1) = -2 * 1 + 3 * 1 = 1 --> p1

    float h2 = t3 - 2 * t2 + t;
    float h3 = t3 - t2;

    return (p0 * h0) + (p1 * h1) + (m0 * h2) + (m1 * h3);
}




//new helper - removes useless interpolations. 
void TargetInterpolator::updateReachedFlagBasedOnDistance(){
    //testing
    float epsilon = 0.1f; //0.5f
    if (FVector::Dist(from, target) <= epsilon)
    { // 0.5cm
        reached = true;
    }
    else
    {
        reached = false;
    }
}






//for door
void TargetInterpolator::resetReachedflag(){
    reached = false;
}