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
	void ComputeConvexHull(std::vector<FVector> &points, FVector paneNormal);
	void ComputeConvexHull(
		std::vector<FVector> &points,
		FVector paneNormal,
		std::vector<FVector> &eliminated
	);
	void ComputeConvexHull(std::vector<FVector> &points, std::vector<FVector> &eliminated, bool findEliminated);

	void ComputeConvexHullAutoProjection(std::vector<FVector> &points);
	void ComputeConvexHullAutoProjection(
		std::vector<FVector> &points,
		std::vector<FVector> &eliminated
	);

private:
	bool IsClockwise(const FVector &a, const FVector &b, const FVector &c);
};
