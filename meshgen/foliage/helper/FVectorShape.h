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
	FVectorShape(const FVectorShape &other, MMatrix &applyOffset);

	~FVectorShape();

	FVectorShape &operator=(const FVectorShape &other);

	void push_back(FVector other);
	void push_back(FVectorShape &other);
	void push_back(std::vector<FVector> &other);

	void moveVerteciesWith(MMatrix &mat);
	void joinMeshData(MeshData &other);


	MeshData createDoubleSidedMesh();
	MeshData closeMeshAtCenter(bool clockwise);

	void copyVertecies(std::vector<MMatrix> &output);

	void randomizeVertecies(int maxdistance);


	void smoothWithBezier();
	void smoothWithBezier(int detailStep);

	int vertexCount();
	void keepVertexCountFromFront(int countLimit);

private:
	std::vector<FVector> vec;

	FVector calculateCenter();

	std::vector<FVector2D> create2DverteciesXY();
};
