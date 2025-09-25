// Fill out your copyright notice in the Description page of Project Settings.


#include "TransformInterpolator.h"

TransformInterpolator::TransformInterpolator()
{
}

TransformInterpolator::~TransformInterpolator()
{
}

void TransformInterpolator::useHermiteSplineInterpolation(bool flag){
    vectorInterpolator.setHermiteSplineFlag(flag);
}

bool TransformInterpolator::hasTargetSetup(){
    return 
    vectorInterpolator.TargetSetupFlag() ||
    rotatorInterpolator.TargetSetupFlag();
    
}

void TransformInterpolator::setTarget(FVector fromIn, FVector totarget, float timeToFrameIn){
    vectorInterpolator.setTarget(fromIn, totarget, timeToFrameIn);
}

void TransformInterpolator::setTarget(
	FVector fromIn, 
	FVector toTarget, 
	FRotator fromRotationIn, 
	FRotator toRotationIn, 
	float timeToFrameIn
){
    setTarget(fromIn, toTarget, timeToFrameIn);
    rotatorInterpolator.setTarget(fromRotationIn, toRotationIn, timeToFrameIn);
}

void TransformInterpolator::setTarget(
    FRotator fromRotationIn, 
    FRotator toRotationIn, 
    float timeToFrameIn
){
    rotatorInterpolator.setTarget(fromRotationIn, toRotationIn, timeToFrameIn);
}

/// @brief override target of a RUNNING ANIMATION!
/// @param totarget 
void TransformInterpolator::overrideTarget(FVector totarget){
    vectorInterpolator.overrideTarget(totarget);
}

void TransformInterpolator::overrideStart(FVector fromtarget){
    vectorInterpolator.overrideStart(fromtarget);
}

void TransformInterpolator::overrideStart(FRotator fromRotationIn){
    rotatorInterpolator.overrideStart(fromRotationIn);
}

void TransformInterpolator::overrideStart(FVector fromtarget, FRotator fromRotationIn){
    overrideStart(fromtarget);
    overrideStart(fromRotationIn);
}
void TransformInterpolator::overrideTarget(FVector totarget, FRotator toRotationIn){
    overrideTarget(totarget);
    overrideTarget(toRotationIn);
}

void TransformInterpolator::overrideTarget(FRotator toRotationIn){
    rotatorInterpolator.overrideTarget(toRotationIn);
}

void TransformInterpolator::overrideStartSpeedRelative(FVector newStart, FRotator newRoation){
    vectorInterpolator.overrideStartSpeedRelative(newStart);
    rotatorInterpolator.overrideTarget(newRoation);
    rotatorInterpolator.overrideTime(vectorInterpolator.TimeToFrame());
}


void TransformInterpolator::overrideStartSpeedRelative(FRotator newRotation){
    rotatorInterpolator.overrideStartSpeedRelative(newRotation);
}


void TransformInterpolator::overrideStartSpeedRelative(FVector newStart){
    vectorInterpolator.overrideStartSpeedRelative(newStart);
}








void TransformInterpolator::resetDeltaTime(){
    vectorInterpolator.resetDeltaTime();
    rotatorInterpolator.resetDeltaTime();
}

/// @brief sets the time to frame and reached flag is set to false
/// @param time 
void TransformInterpolator::setNewTimeToFrame(float time){
    vectorInterpolator.overrideTime(time);
    rotatorInterpolator.overrideTime(time);
}


bool TransformInterpolator::hasReachedTarget(){
    return vectorInterpolator.hasReachedTarget() && rotatorInterpolator.hasReachedTarget(); //if not setup = true
}

FVector TransformInterpolator::interpolate(float DeltaTime){
    return vectorInterpolator.interpolate(DeltaTime);
}


FRotator TransformInterpolator::interpolateRotationOnly(float DeltaTime){
    return rotatorInterpolator.interpolate(DeltaTime);
}


FVector TransformInterpolator::interpolate(float DeltaTime, FRotator &rotationOutgoing){
    FVector outpos = interpolate(DeltaTime);
    rotationOutgoing = rotatorInterpolator.interpolate(DeltaTime);
    return outpos;
}

MMatrix TransformInterpolator::interpolateAndGenerateTransform(float DeltaTime){
    MMatrix outMatrix;
    FRotator rotatorOut;
    FVector pos = interpolate(DeltaTime, rotatorOut);
    outMatrix.setRotation(rotatorOut);
    outMatrix.setTranslation(pos);
    return outMatrix;
}

float TransformInterpolator::TimeToFrame(){
    return std::max(vectorInterpolator.TimeToFrame(), rotatorInterpolator.TimeToFrame()); //larger one left.
}

FVector TransformInterpolator::readFromPosition(){
    return vectorInterpolator.readFrom();
}

FVector TransformInterpolator::readToPosition(){
    return vectorInterpolator.readTarget();
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


void TransformInterpolator::overrideStartWorld(FVector targetIn){
    fromWorld = targetIn;
}

void TransformInterpolator::overrideTargetWorld(FVector targetIn){
    targetWorld = targetIn;
    worldtargetSetup = true;
}

/// @brief overrides the starting position of the interpolation in world position
/// if the world target is setup, the local target will be also updated, relative to the actor matrix transform
/// @param newStart new world frame start
/// @param actor actor with rotation and translation
void TransformInterpolator::overrideStartWorldSpeedRelative(FVector newStart, MMatrix &actor){

    MMatrix inverse = actor.jordanInverse();
    if (!worldtargetSetup)
    {
        //translate start to local and override start
        FVector startLocal = inverse * newStart;
        //overrideStartSpeedRelative(startLocal);
        vectorInterpolator.overrideStartSpeedRelative(startLocal);
        return;
    }else{
        
        //translate start and target to local and override both, target must be available
        FVector targetLocalNew = inverse * targetWorld;
        overrideTarget(targetLocalNew);
        FVector startLocal = inverse * newStart;
        //overrideStartSpeedRelative(startLocal);
        vectorInterpolator.overrideStartSpeedRelative(startLocal);
        return;
    }
}


//for door
void TransformInterpolator::resetReachedflag(){
    rotatorInterpolator.resetReachedflag();
    vectorInterpolator.resetReachedflag();
}