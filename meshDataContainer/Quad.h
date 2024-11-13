// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * saves one instance of a quad
 */
class P2_API Quad
{
public:
	Quad(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d
	);
	~Quad();

	TArray<FVector> &readVertecies();

	TArray<int32> &readTriangles();

	FVector center();

	bool isWithinQuad(FVector &other);

private:
	TArray<FVector> vertecies;
	TArray<int32> triangles;

	void createQuad(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d
	);
	void buildTriangle(
		FVector &a, 
		FVector &b, 
		FVector &c,
		TArray<FVector> &output,
		TArray<int32> &trianglesOutput
	);

	bool isWithinTriangle(FVector &target, FVector &a, FVector &b, FVector &c);
	bool isInRangeOfAB(FVector &a, FVector &b, FVector &compare);

	//collision check
	bool doesHit(
		int xCheck,
		int yCheck,
		int xA,
		int yA,
		int xB,
		int yB
	);
	float dotProduct(
		int x0,
		int y0,
		int x1,
		int y1);
};
