// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API GrahamScan
{
public:
	GrahamScan();
	~GrahamScan();

	void ComputeConvexHull(std::vector<FVector> &points);

private:
	bool IsClockwise(const FVector &a, const FVector &b, const FVector &c);
};
