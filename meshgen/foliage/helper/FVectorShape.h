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

	/**
	 * very important function to append this mesh to other data linearly
	 */
	void joinMeshData(MeshData &other);


	void push_back(FVector other);
	void push_back(FVectorShape &other);
	void push_back(std::vector<FVector> &other);

	void moveVerteciesWith(MMatrix &mat);
	


	MeshData createDoubleSidedMesh();
	MeshData closeMeshAtCenter(bool clockwise);

	void copyVertecies(std::vector<MMatrix> &output);

	void randomizeVertecies(int maxdistance);


	void smoothWithBezier();
	void smoothWithBezier(int detailStep);

	int vertexCount();
	void keepVertexCountFromFront(int countLimit);


	void makeCenterPivot();

	void createRandomNewSmoothedShape(int sizeXYMax, int smoothStep);
	void createRandomNewSmoothedShapeClamped(int sizeXYMax, int smoothStep);
	void sortVerteciesOnXAxis();
	void floorAllCoordinateValues();

	std::vector<FVector> vectorCopy();

	void createCircleShape(int radius, int detail);
	void createCircleShapeOnPitchRotation(int radius, int detail);
	void createHalfCircleShapeOnPitchRotation(int radius, int detail);
	void createSpikedCircleShape(int radius, int radiusInner, int detail);
	void createQuadShape(int sizeTotal);

	static MeshData createSphere(int radius, int detail, bool faceOutside);
	static MeshData createHalfSphereCuttedOff(
		int radius,
		int fullCircleDetail,
		bool faceOutside,
		FVector normalDir,
		std::vector<FVector> &outlineShape,
		MMatrix extraOffset
	);

private:
	std::vector<FVector> vec;

	FVector calculateCenter();

	std::vector<FVector2D> create2DverteciesXY();

	void clampVector(FVector &pos, int minValue, int maxValue);

};
