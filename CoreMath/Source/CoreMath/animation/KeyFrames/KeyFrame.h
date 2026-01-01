// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class COREMATH_API KeyFrame
{
public:
	KeyFrame(FVector positionIn, float timeToFrameIn);
	KeyFrame(FVector positionIn, float timeToFrameIn, bool groundedIn);
	~KeyFrame();

	FVector readposition();
	float readTimeToFrame();
	bool mustBeGrounded();

	void setTimeToFrame(float timeToFrameIn);

	float distanceTo(KeyFrame &other);

private:
	FVector position;
	float timeToFrame;
	bool grounded = false;
};
