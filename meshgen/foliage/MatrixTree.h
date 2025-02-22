// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/entities/customIk/MMatrix.h"
#include "p2/meshgen/MeshData.h"
#include "IndexChain.h"

/**
 * 
 */
class P2_API MatrixTree
{
public:
	MatrixTree();
	~MatrixTree();

	void generate(int height, int perCmMatrix);

	void generateMesh();

	MeshData &meshDataByReference();

private:
	int stemCount;
	MMatrix stemTop();

	bool wasModified = false; //save for rebuild

	/** 
	 * Alle matrizen sollten in einem vektor festgehalten werden
	 * Die chains sollten mit indices erstellt werden
	 */
	std::vector<MMatrix> matrices;
	std::vector<IndexChain> indexChains;

	std::vector<FVector> shapeByEnum();
	std::vector<MMatrix> buildChain(IndexChain &indexChain);

	bool indexIsValid(int index);
	MMatrix &matrixByIndex(int index);
	MMatrix identityMatrix;

	MeshData ownMeshData;

	/**
	 * sollte hier auch das mesh selber drin gespeichert werden?
	 */

	void moveVerteciesFromLocalToWorld(MMatrix &mat, std::vector<FVector> &vector);
	void wrapWithMesh(std::vector<MMatrix> &matricesIn, MeshData &meshToAddTo);
	void join(std::vector<FVector> &lower, std::vector<FVector> &upper, MeshData &mesh);
};
