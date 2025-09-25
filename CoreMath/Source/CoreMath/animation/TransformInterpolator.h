// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "RotationInterpolator.h"

#include "FVectorInterpolator.h"
#include "FRotatorInterpolator.h"


/**
 * 
 */
class COREMATH_API TransformInterpolator
{
private:
	FVectorInterpolator vectorInterpolator;
	FRotatorInterpolator rotatorInterpolator;

public:
	TransformInterpolator();
	~TransformInterpolator();

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
	

	float skalar();

	FVector fromWorld;
	FVector targetWorld;
	bool worldtargetSetup = false;
};
