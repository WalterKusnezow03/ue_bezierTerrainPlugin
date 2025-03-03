// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/entities/customIk/MMatrix.h"
#include "p2/meshgen/MeshData.h"
#include "p2/meshgen/foliage/helper/IndexChain.h"
#include "p2/meshgen/foliage/helper/TreeProperties.h"
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

	void generate(ETerrainType type);

	MeshData &meshDataStemByReference();
	MeshData &meshDataLeafByReference();

private:
	void clean();


	void processAndGenerate(TreeProperties &properties);
	void generateMesh();
	void generateLeafs(TreeProperties &properties);
	void generateLeaf(MMatrix &offset);
	void generateCactusSpikes();

	MeshData stemMeshData;
	MeshData leafMeshData;

	ETreeType treeType;


	MMatrix stemTop;

	/// @brief vector for all matricies to build upon
	std::vector<MMatrix> matrices;

	/// @brief index chains to save subtrees instead of copying all matrices
	std::vector<IndexChain> indexChains;
	
	MMatrix identityMatrix;

	std::vector<MMatrix> leafTops;

	std::vector<FVectorShape> StemShapeByEnum(ETreeType type);
	void buildChain(IndexChain &indexChain);

	FVectorShape leafShapeByEnum(ETreeType type);

	bool indexIsValid(int index);
	MMatrix &matrixByIndex(int index);
	void randomRotationForAllMatrices();

	void wrapWithMesh(std::vector<MMatrix> &matricesIn, MeshData &meshToAddTo);

	void createSubTrees(MMatrix &offset, TreeProperties &propeties);
	IndexChain createSubTree(MMatrix &offset, TreeProperties &propeties);

	
	int rotationRangeByEnum(ETreeType type);
	

	MMatrix randomRotator();
	MMatrix randomRotator(int lower, int heigher);




	void loadProperties();
	void addPropertyToMap(TreeProperties &property);
	
	/// @brief all property map
	std::map<ETerrainType, std::vector<TreeProperties>> terrainPropertyMap;
	TreeProperties defaultProperty;
	
	TreeProperties &findProperty(ETerrainType type);
	
};
