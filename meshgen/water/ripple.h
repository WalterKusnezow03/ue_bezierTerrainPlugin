// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API ripple
{
public:
	ripple(FVector &impactPoint);
	~ripple();

	void init(FVector &impactPoint);
	void increaseRadius(float deltaTime);
	bool timeExceeded();

	void changeHeightBasedOnDistance(FVector &vertex);

private:
	float maxlifeTime = 10.0f;
	float velocity = 100.0f;
	float time = 0.0f;
	float radius;
	FVector impact;
};
