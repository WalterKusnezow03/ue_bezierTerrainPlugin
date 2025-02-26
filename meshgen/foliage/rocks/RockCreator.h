// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/meshgen/foliage/helper/FVectorShape.h"
#include "p2/meshgen/MeshData.h"

/**
 * 
 */
class P2_API RockCreator
{
public:
	RockCreator();
	~RockCreator();

	MeshData createMesh();
	MeshData createMesh(int sizeX, int sizeY, int detailStep, int layers, int heightStep);

private:
	
	FVectorShape createShape(int detailStep, int sizeX, int sizeY);

	

	void addVertecies(
		FVectorShape &shape,
		FVector &directionWithDistance,
		FVector &offset,
		int count
	);
};
