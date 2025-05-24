// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TERRAINPLUGIN_API ripple
{
public:
	ripple(FVector &impactPoint);
	ripple(FVector &impactPoint, float newMaxRadius);
	~ripple();

	ripple(const ripple &other);
	ripple &operator=(const ripple &other);

	void setMaxRadius(float rad);

	void init(FVector &impactPoint, float maxRadiusIn);
	void Tick(float deltaTime);
	bool timeExceeded();

	void changeHeightBasedOnDistance(FVector &vertex, FVector &offsetActor);

private:
	float waveHeightBasedOnTime();

	float maxlifeTime = 10.0f;
	float velocity = 100.0f;
	float time = 0.0f;
	float radius;
	float maxRadius = 100.0f;
	FVector impact;

	int waveHeight = 50;

	int rippleWidth = 50;
};
