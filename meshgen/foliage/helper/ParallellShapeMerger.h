// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * can merge 2 circular shapes for example and create all triangles between the shapes
 * while having them in a logical and sorted order
 */
class P2_API ParallellShapeMerger
{
public:
	ParallellShapeMerger();
	~ParallellShapeMerger();

	void createParallelSortedShape(
		std::vector<FVector> &baseVectorIn,
		std::vector<FVector> &vectorToAlginAndSort
	);



	std::vector<FVector> &baseVectorReference();
	std::vector<FVector> &alignedVectorReference();
	std::vector<FVector> &triangleBufferReference();

private:

	void fillMissingAlignedVertecies(std::vector<FVector> &other);
	
	
	void generateTriangleBuffer();
	

	std::vector<FVector> baseVector;
	std::vector<FVector> vectorAligned;


	std::vector<FVector> triangleBuffer;

	const int errorValue = -1;

	//template function
	template <typename T>
    bool isInBounds(std::vector<T>& vec, int index){
		return index >= 0 && index < vec.size();
	}




	//new sorting
	void sortPointsByDistance(
		std::vector<FVector> &points
	);
	void sortPointsByDistance(
		std::vector<FVector> &points,
		FVector startingPoint,
		FVector nextToStartingPoint
	);

	bool findClosestPointTo(
		std::vector<FVector> &points,
		std::vector<bool> &usedIndices,
		FVector &other,
		FVector &outpoint
	);

	bool findClosestPointToAndMatchDirection(
		std::vector<FVector> &points,
		std::vector<bool> &usedIndices,
		FVector &closestPointSearchedFor,
		FVector &outpoint,
		FVector &directionToMatch
	);
};