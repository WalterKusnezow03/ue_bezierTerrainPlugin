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
	~FVectorShape();

	void push_back(FVector other);

	void moveVerteciesWith(MMatrix &mat);

	MeshData join(FVectorShape &other);



private:
	std::vector<FVector> vec;
};
