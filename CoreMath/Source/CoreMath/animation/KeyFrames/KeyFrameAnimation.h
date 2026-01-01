// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreMath/animation/TransformInterpolator.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "KeyFrame.h"

/**
 * will store keyframes for a bone movement for example
 */
class COREMATH_API KeyFrameAnimation
{
public:
	KeyFrameAnimation();
	KeyFrameAnimation(bool loopIn);
	~KeyFrameAnimation();

	void addFrame(FVector position, float timeFromLastFrame);
	void addFrame(FVector position, float timeFromLastFrame, bool mustBeGrounded);
	void addFrame(
		FVector position,
		float timeFromLastFrame,
		bool mustBeGrounded,
		float clampDistance
	);
	
	FVector interpolate(float DeltaTime);
	FVector interpolate(float DeltaTime, FVector currentPos);
	FVector interpolateWorld(
		float DeltaTime,
		FVector currentPosWorld,
		MMatrix &actor
	);

	bool nextFrameMustBeGrounded();
	FVector readNextFrame();
	
	
	float totalLength();
	bool nextFrameIsProjected();
	bool reachedLastFrameOfAnimation();
	void overrideCurrentStartingFrame(FVector &somePoisition);
	void overrideNextFrame(FVector &framePos);
	
	void restart();

	void skipAnimationOnce(FVector start, FVector end);
	void skipAnimationOnceWorld(MMatrix &actor, FVector start, FVector end);







	/**
	 * --- START new section for rotation on frames! ---
	 */
	void rotateNextFrames(float singedAngleYaw);
	void resetRotationOnFramesFlag();

	void resetAnimationToStartAndResetRotation();


	//new 
	void useHermiteSplineInterpolation(bool flag);

private:
	void addRotationToFrame(FVector &localFrameToRotate);
	bool rotateFramesBasedOnAngle = false;
	MMatrix rotateFramesMatrix;
	/**
	 * --- END new section for rotation on frames! ---
	 */


	bool DEBUGDRAW_RAYCAST = false;

	float raycastVerticalStartOffsetAdd = 250.0f; //400.0f
	float raycastScaleVector = 5000.0f;

	FVector latestInterpolation;
	bool loop = true;
	bool restarted = true;
	bool reachedEndFrameFlag = false;

	float totalLengthSave = 0.0f;
	

	bool frameIsProjected = false;

	

	std::vector<KeyFrame> frames; //might be replaced with frame class with time stamp
	float deltaTime = 0.0f;
	float lastDeltatime = 0.0f;

	int frameIndex = 0;
	int nextFrameIndex = 1;

	void updateFrameIndex();
	bool canAnimate();
	bool hasAnyFrames();




	class TransformInterpolator interpolator;
	// class TransformInterpolator interpolator;
	void updateFrameInterpolator();



	bool performRaycast(UWorld *world, FVector &Start, FVector &dir, FVector &outputHit);

	

	void updateAverageVelocity();
	float averageVelocityOfFrames = 0.0f;
	float linearVelocity(FVector &a, FVector &b, float timeBetween);

public:
	float averageVelocity();
	void scaleToVelocityInCms(float VcmPerSecond);
};
