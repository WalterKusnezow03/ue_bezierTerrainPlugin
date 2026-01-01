// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreMath/animation/TransformInterpolator.h"
#include "KeyFrameAnimation.h"
#include "CoreMath/Matrix/MMatrix.h"


KeyFrameAnimation::KeyFrameAnimation()
{
    frameIndex = 0;
    nextFrameIndex = 1;
    totalLengthSave = 0.0f;
    loop = true;

}


/// @brief constructor
/// @param loopIn if true: last frame will be also interpolated to the first once one cycle is complete
KeyFrameAnimation::KeyFrameAnimation(bool loopIn){
    frameIndex = 0;
    nextFrameIndex = 1;
    totalLengthSave = 0.0f;
    loop = loopIn;
}


KeyFrameAnimation::~KeyFrameAnimation()
{
}



void KeyFrameAnimation::useHermiteSplineInterpolation(bool flag){
    interpolator.useHermiteSplineInterpolation(flag);
}

/// @brief add a frame to the animation at a position and distance to previous frame
/// @param position position for frame
/// @param timeFromLastFrame time to keep from last frame in seconds
void KeyFrameAnimation::addFrame(FVector position, float timeFromLastFrame){
    addFrame(position, timeFromLastFrame, false);
}

void KeyFrameAnimation::addFrame(FVector position, float timeFromLastFrame, bool mustBeGrounded){
    totalLengthSave += timeFromLastFrame;
    frames.push_back(KeyFrame(position, timeFromLastFrame, mustBeGrounded));

    updateAverageVelocity();
}

void KeyFrameAnimation::addFrame(
    FVector position,
    float timeFromLastFrame,
    bool mustBeGrounded,
    float clampDistance
){
    FVector zeroVec(0, 0, 0);
    if(FVector::Dist(zeroVec, position) > clampDistance){
        position = position.GetSafeNormal() * clampDistance;
    }

    totalLengthSave += timeFromLastFrame;
    frames.push_back(KeyFrame(position, timeFromLastFrame, mustBeGrounded));
    
    updateAverageVelocity();
}





/// @brief total length time of the animation
/// @return total length time of the animation
float KeyFrameAnimation::totalLength(){
    return totalLengthSave;
}

/// @brief returns whether the end frame was reached, only is flagged if the animation
/// was not initialized with loop, NOT LOOP!
/// @return reached last frame flag
bool KeyFrameAnimation::reachedLastFrameOfAnimation(){
    if(reachedEndFrameFlag){
        reachedEndFrameFlag = false;
        return true;
    }
    return false;
}



/// @brief returns if enough frames are available for an animation (at least 2)
/// @return has enough or not
bool KeyFrameAnimation::hasAnyFrames(){
    return frames.size() >= 2; //must be at least 2!
}


/**
 * 
 * ---- INTERPOLATE DEFAULT ----
 * 
 */

FVector KeyFrameAnimation::interpolate(float DeltaTime){

    if(hasAnyFrames()){
    
        if (interpolator.hasTargetSetup() == false){
            updateFrameInterpolator();
        }

        FVector interpolated = interpolator.interpolate(DeltaTime);
        

        if(interpolator.hasReachedTarget()){    
            updateFrameIndex();
            updateFrameInterpolator();
        }

        return interpolated;
    }
    //DebugHelper::showScreenMessage("not enough frames!");
    return FVector(0, 0, 0);
}   






/// @brief updates the frame index and end frame flag if reached
void KeyFrameAnimation::updateFrameIndex(){
    frameIndex = (frameIndex + 1) % frames.size();
    nextFrameIndex = (frameIndex + 1) % frames.size();

    bool isEnd = (nextFrameIndex == 0) || (frameIndex == frames.size() - 1);
    if (isEnd && !loop)
    {
        frameIndex = 0;
        nextFrameIndex = 1;
        //DebugHelper::showScreenMessage("reached end frame!", FColor::Orange);
        reachedEndFrameFlag = true;
    }
    if(isEnd){
        //reset rotation!
        resetRotationOnFramesFlag();
    }

}

/// @brief call update AFTER increase / index update
void KeyFrameAnimation::updateFrameInterpolator(){

    KeyFrame &currentFrame = frames.at(frameIndex);
    KeyFrame &nextFrame = frames.at(nextFrameIndex);

    FVector currentFramePosition = currentFrame.readposition();

    //copy latest reached position if possible
    if(interpolator.hasTargetSetup() && frameIndex != 0){
        currentFramePosition = interpolator.readToPosition();
    }

    interpolator.setTarget(
        currentFramePosition, //currentFrame.readposition();
        nextFrame.readposition(),
        nextFrame.readTimeToFrame()
    );

    //reset frame projected status
    frameIsProjected = false;
}

void KeyFrameAnimation::resetAnimationToStartAndResetRotation(){
    if(hasAnyFrames()){
        frameIndex = 0;
        nextFrameIndex = 1;
        
        KeyFrame &currentFrame = frames.at(frameIndex);
        KeyFrame &nextFrame = frames.at(nextFrameIndex);
        FVector currentFramePosition = currentFrame.readposition();

        interpolator.setTarget(
            currentFramePosition, //currentFrame.readposition();
            nextFrame.readposition(),
            nextFrame.readTimeToFrame()
        );

        //reset frame projected status
        frameIsProjected = false;
    }
    resetRotationOnFramesFlag();
}

/// @brief will tell if the next key frame must be grounded or not
/// @return must be grounded
bool KeyFrameAnimation::nextFrameMustBeGrounded(){
    if(hasAnyFrames()){
        KeyFrame &nextFrame = frames.at(nextFrameIndex);
        return nextFrame.mustBeGrounded();
    }
    return false;
}

/// @brief copies the next frames position AND ROTATES IT IF NEEDED!
/// @return position of next frame of animation
FVector KeyFrameAnimation::readNextFrame(){
    if(hasAnyFrames()){
        KeyFrame &nextFrame = frames.at(nextFrameIndex);

        FVector position = nextFrame.readposition();
        addRotationToFrame(position);
        return position;

        //old
        //return nextFrame.readposition();
    }
    return FVector(0, 0, 0);
}



/// @brief override the next frame / target value, animation is not overriden, individual to current next
/// frame!
/// @param framePos 
void KeyFrameAnimation::overrideNextFrame(FVector &framePos){
    interpolator.overrideTarget(framePos);
    frameIsProjected = true;
}





bool KeyFrameAnimation::nextFrameIsProjected(){
    return frameIsProjected;
}




/// @brief overrides the starting position of the current interpolation
void KeyFrameAnimation::overrideCurrentStartingFrame(FVector &somePosition){
    interpolator.overrideStart(somePosition); //override start pos
}

/// @brief skip the animation with a and b interpolated keys, after interpolation
/// the reached end frame flag is true as expected if not looping animation
/// @param start start frame
/// @param end end frame
void KeyFrameAnimation::skipAnimationOnce(FVector start, FVector end){
    interpolator.overrideStart(start);
    interpolator.overrideTarget(end);
    interpolator.resetDeltaTime();
    frameIndex = frames.size() - 2; 
    //next frame will be -1, end will be reached
    //next frame will be 0 by then

}

/// @brief will override the world and local animation
/// @param actor actor world pos to make frames relative, both world and local keyframes are made
/// @param start world pos, will be relative to actor
/// @param end world pos, will be relative to actor
void KeyFrameAnimation::skipAnimationOnceWorld(MMatrix &actor, FVector start, FVector end){
    //world
    interpolator.overrideStartWorld(start);
    interpolator.overrideTargetWorld(end);

    MMatrix inverse = actor.jordanInverse();
    start = inverse * start;
    end = inverse * end;

    //relative
    skipAnimationOnce(start, end); //relative override
}




/**
 * 
 * ---- PROJECTION SECTION ----
 * 
 */





/// @brief performs a raycast from a start, into a direction, with a max length in this method
/// @param Start start position
/// @param dir direction of interest
/// @param outputHit output hit if any hit happened
/// @return bool anything was hit
bool KeyFrameAnimation::performRaycast(UWorld *world, FVector &Start, FVector &dir, FVector &outputHit) {

    if(world == nullptr){
        return false;
    }

	FVector End = Start + dir * raycastScaleVector; // gx = A + r (B - A)

	// Perform the raycast
	FHitResult HitResult;

	FCollisionQueryParams ignoreParams;
	
	//params to avoid hitting own bones, weapons, etc
	/*if(EntityManager *e = AworldLevel::entityManager()){
		//example for getting all
		ignoreParams = e->getIgnoredRaycastParams();
	}*/
	bool bHit = world->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, ignoreParams);
	

	//anything was hit
	if (bHit){
		outputHit = HitResult.ImpactPoint; //write impactpoint to output
		return true;
	}

	return false;
}










/**
 * 
 * ---- INTERPOLATE WITH LOCAL UPDATE ----
 * 
 */

/// @brief interpolate with considering current pos 
/// @param DeltaTime delta time since last frame
/// @param currentPos current local position
/// @return 
FVector KeyFrameAnimation::interpolate(float DeltaTime, FVector currentPos){

    if(hasAnyFrames()){
    
        if (interpolator.hasTargetSetup() == false){
            updateFrameInterpolator();
        }


        //interpolator.overrideStart(currentPos); //debug disabled

        interpolator.overrideStartSpeedRelative(currentPos);
        FVector interpolated = interpolator.interpolate(DeltaTime);

        if(interpolator.hasReachedTarget()){
            //DebugHelper::logMessage("debugmotion interpolator update");
            updateFrameIndex();
            updateFrameInterpolator();
        }

        return interpolated;
    }
    //DebugHelper::showScreenMessage("not enough frames!");
    return FVector(0, 0, 0);
}   







/**
 * 
 * 
 * ---- INTERPOLATE WITH WORLD UPDATE ----
 * 
 * 
 */


/// @brief will interpolate to target World if possible and switch to local / default animation
/// if an issue occured
/// @param DeltaTime delta time since last frame
/// @param currentPosWorld current position in world
/// @param actor actor matrix in world (with rotation) to update the world frames to local ones
/// @return interpolation based on actor matrix and current position (of end effector) in world
FVector KeyFrameAnimation::interpolateWorld(
    float DeltaTime, 
    FVector currentPosWorld, 
    MMatrix &actor //limb start matrix
){

    if(hasAnyFrames()){
    
        if (interpolator.hasTargetSetup() == false){
            updateFrameInterpolator();
        }


        interpolator.overrideStartWorldSpeedRelative(currentPosWorld, actor); //will transform to local internally
        FVector interpolated = interpolator.interpolate(DeltaTime);

        if(interpolator.hasReachedTarget()){
            //DebugHelper::logMessage("debugmotion interpolator update");
            updateFrameIndex();
            updateFrameInterpolator();
        }

        return interpolated;
    }
    //DebugHelper::showScreenMessage("not enough frames!");
    return FVector(0, 0, 0);


}







/**
 * 
 * ---- new rotation on frames setup ----
 * 
 */


/// @brief rotates this animations keyframe in a singed yaw angle,
/// rotation will be reset once completed (reaching the end frame)
/// @param signedAngleYawDegree 
void KeyFrameAnimation::rotateNextFrames(float signedAngleYawDegree){
    if(signedAngleYawDegree == 0.0f){
        rotateFramesBasedOnAngle = false;
        return;
    }
    rotateFramesBasedOnAngle = true;
    rotateFramesMatrix.resetRotation();
    rotateFramesMatrix.yawRadAdd(MMatrix::degToRadian(signedAngleYawDegree));
    rotateFramesMatrix.setTranslation(0, 0, 0);
}

void KeyFrameAnimation::resetRotationOnFramesFlag(){
    rotateFramesBasedOnAngle = false;
}

void KeyFrameAnimation::addRotationToFrame(FVector &localFrameToRotate){
    if(rotateFramesBasedOnAngle){
        localFrameToRotate = rotateFramesMatrix * localFrameToRotate;
    }
}









// --- velocity tracking section of all frames ---



void KeyFrameAnimation::updateAverageVelocity(){
    float sum = 0.0f;
    for (int i = 1; i < frames.size(); i++){
        KeyFrame &prevFrame = frames.at(i-1);
        KeyFrame &frame = frames.at(i);
        
        FVector posA = prevFrame.readposition();
        FVector posB = frame.readposition();
        float time = frame.readTimeToFrame();

        sum += linearVelocity(posA, posB, time);
    }
    if(frames.size() > 0){
        sum /= (frames.size() - 1); //-1 strecken abschnitte
    }
    averageVelocityOfFrames = sum;
}

float KeyFrameAnimation::linearVelocity(FVector &a, FVector &b, float timeBetween){
    //v = m / s
    timeBetween = std::abs(timeBetween);
    timeBetween = std::max(timeBetween, 0.0000001f); //avoid div by zero
    float dist = FVector::Dist(a, b);
    float velocity = dist / timeBetween;
    return velocity;
}

float KeyFrameAnimation::averageVelocity(){
    //if new frames added: recreate
    return averageVelocityOfFrames;
}



/// @brief scale the velocity of the animation to a constant centimeter per second
/// @param VcmPerSecond velocity in cms to have, must not be 0.0f! - otherwise not executed
/// value will be made a absolute value!
void KeyFrameAnimation::scaleToVelocityInCms(float VcmPerSecond){
    VcmPerSecond = std::abs(VcmPerSecond);
    if(VcmPerSecond < 0.00001f){
        return;
    }

    //is tested
    for (int i = 1; i < frames.size(); i++){
        KeyFrame &prev = frames[i - 1];
        KeyFrame &current = frames[i];
        float dist = current.distanceTo(prev);
        
        //v = m/s
        /**
         * s := unknown , m := distance, v1 := targetVelocity
         * 
         * v1 = m/s
         * v1 * s = m
         * s = m / v1
        */
        float oldTime = current.readTimeToFrame();
        float newTime = dist / VcmPerSecond;
        current.setTimeToFrame(newTime);

        
    }
    updateAverageVelocity();

}