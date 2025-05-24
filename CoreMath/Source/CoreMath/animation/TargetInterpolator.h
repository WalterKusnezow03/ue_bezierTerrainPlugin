// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"
/**
 * 
 */
class COREMATH_API TargetInterpolator
{
public:
	TargetInterpolator();
	~TargetInterpolator();

	void setTarget(FVector from, FVector toTarget, float timeToFrameIn);
	void setTarget(
		FVector fromIn, 
		FVector toTarget, 
		FRotator fromRotationIn, 
		FRotator toRotationIn, 
		float timeToFrameIn
	);

	void setTarget(
		FRotator fromRotationIn, 
		FRotator toRotationIn, 
		float timeToFrameIn
	);
	
	void overrideTarget(FVector totarget);
	void overrideTarget(FRotator toRotationIn);
	void overrideStart(FVector fromTarget);
	void overrideStart(FRotator fromRotationIn);
	void overrideStartSpeedRelative(FVector newStart);
	void overrideStartSpeedRelative(FRotator newRotation);
	void overrideStartSpeedRelative(FVector newStart, FRotator newRoation);

	void overrideStart(FVector fromtarget, FRotator fromRotationIn);
	void overrideTarget(FVector totarget, FRotator toRotationIn);



	//void overrideStartSpeedRelative(FRotator rotator);
	//void overrideTarget(FRotator rotator);

	bool hasReachedTarget();
	bool hasTargetSetup();
	void resetReachedflag();

	float TimeToFrame();
	void resetDeltaTime();

	FVector interpolate(float DeltaTime);
	FVector interpolate(float DeltaTime, FRotator &rotationOutgoing);
	FRotator interpolateRotationOnly(float DeltaTime);
	MMatrix interpolateAndGenerateTransform(float DeltaTime);
	

	static FRotator interpolationRotation(FRotator fromIn, FRotator toIn, float skalar);
	static FVector interpolation(FVector from, FVector to, float skalar);
	 
	FVector readFromPosition();
	FVector readToPosition();

	void setNewTimeToFrame(float time);



	void overrideStartWorld(FVector targetIn);
	void overrideTargetWorld(FVector targetIn);
	void overrideStartWorldSpeedRelative(FVector newStart, MMatrix &actor);

	void useHermiteSplineInterpolation(bool flag);

protected:
	bool useHermiteSplineFlag = true; //true by default

	FVector from;
	FVector target;

	FVector prev;

	FVector fromWorld;
	FVector targetWorld;

	FRotator fromRotation;
	FRotator toRotation;

	float deltaTime = 0.0f;
	float timeToFrame = 0.0f;
	bool reached = false;
	
	float skalar();

	bool targetSetup = false;
	bool worldtargetSetup = false;

	static float rotationDirectionShorter(float a, float b);
	static float wrapAngle180(float angle);

	float shorterAngleSum(FRotator &a, FRotator &b);



	FVector HermiteInterpolate(float t);
	FVector HermiteInterpolate(
		FVector &p0,
		FVector &p1,
		FVector &m0, // tangente at p0
		FVector &m1, // tangente at p1
		float t
	);

	void updateReachedFlagBasedOnDistance();
};
