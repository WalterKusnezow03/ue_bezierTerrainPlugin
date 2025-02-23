// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/meshgen/MeshData.h"

/**
 * 
 */
class P2_API FVectorShape
{
public:
	FVectorShape();
	FVectorShape(const FVectorShape &other);
	~FVectorShape();

	FVectorShape &operator=(const FVectorShape &other);

	void push_back(FVector other);
	void push_back(FVectorShape &other);

	void moveVerteciesWith(MMatrix &mat);


	MeshData join(FVectorShape &other);

	MeshData createDoubleSidedMesh();

	void copyVertecies(std::vector<MMatrix> &output);

private:
	std::vector<FVector> vec;
};
