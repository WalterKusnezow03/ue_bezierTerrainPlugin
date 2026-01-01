// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class COREMATH_API timer
{
public:
	timer();
	timer(float time, bool resetsItselfIn);
	virtual ~timer();

	void Begin(float time);
	void Begin(float time, bool resetsItselfIn);
	virtual void Tick(float deltaTime);
	
	virtual bool timesUp();
	virtual bool TickWithTimesUpReset(float deltatime);

	float currentTimeLeft(){
		return timeLeft;
	}

	virtual float scalar();

private:
	float initialTime;
	float timeLeft;
	bool resetsItself = false;
};
