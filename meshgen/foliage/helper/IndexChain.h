// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/entities/customIk/MMatrix.h"

/**
 * saves an index and matrix chain for the trees
 */
class P2_API IndexChain
{
public:
	IndexChain();
	~IndexChain();

	IndexChain(const IndexChain &other);
	IndexChain &operator=(const IndexChain &other);

	void addIndex(int i);
	std::vector<int> &indexChain();

	void addMatrix(MMatrix &other);
	std::vector<MMatrix> &matrixChain();

	void setOffsetMatrix(MMatrix &mat);

	MMatrix &offsetMatrixRef();

	MMatrix &endMatrixRef();
	void updateEndMatrix(MMatrix &other);

	void resetMatrices();

	void setRecursionLevel(int level);
	
	MMatrix scaleXYMatrixFromrecursionLevel();

private:
	std::vector<int> indexChainVector;

	MMatrix offsetMatrix;
	MMatrix endMatrix;
	std::vector<MMatrix> matrixChainVector;

	int internalRecursionLevel = 0;

	int getRecursionLevel();
	float scaleFractionByRecursionLevel();

};
