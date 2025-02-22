// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/entities/customIk/MMatrix.h"
#include "p2/meshgen/MeshData.h"
#include "IndexChain.h"
#include "p2/meshgen/foliage/helper/FVectorShape.h"
#include "ETreeType.h"

/**
 * 
 */
class P2_API MatrixTree
{
public:
	MatrixTree();
	~MatrixTree();

	void generate(int height, int perCmMatrix, ETreeType tyep);

	void generateMesh();

	MeshData &meshDataByReference();

private:
	void randomRotationForAllMatrices();

	ETreeType treeType;
	int stemCountTop;
	MMatrix stemTop();

	bool wasModified = false; //save for rebuild

	/** 
	 * Alle matrizen sollten in einem vektor festgehalten werden
	 * Die chains sollten mit indices erstellt werden
	 */
	std::vector<MMatrix> matrices;
	std::vector<IndexChain> indexChains;

	std::vector<FVectorShape> shapeByEnum(ETreeType type);
	std::vector<MMatrix> buildChain(IndexChain &indexChain);

	bool indexIsValid(int index);
	MMatrix &matrixByIndex(int index);
	MMatrix identityMatrix;

	MeshData ownMeshData;

	/**
	 * sollte hier auch das mesh selber drin gespeichert werden?
	 */
	void wrapWithMesh(std::vector<MMatrix> &matricesIn, MeshData &meshToAddTo);

	void createSubTrees(MMatrix &offset, int partsPerTree, int count);
	IndexChain createSubTree(MMatrix &offset, int parts);

	int subTreeCountByEnum(ETreeType type);
	int rotationRangeByEnum(ETreeType type);
};
