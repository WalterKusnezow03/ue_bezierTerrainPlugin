// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * saves an index and matrix chain for the trees
 */
class P2_API IndexChain
{
public:
	IndexChain();
	~IndexChain();

	void addIndex(int i);
	std::vector<int> &indexChain();

	void setOffsetMatrix(MMatrix &mat);

	MMatrix &offsetMatrixRef();

private:
	std::vector<int> indexChainVector;
	MMatrix offsetMatrix;
};
