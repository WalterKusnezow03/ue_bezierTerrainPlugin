// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"
#include "terrainPlugin/meshgen/foliage/helper/IndexChain.h"
#include "terrainPlugin/meshgen/foliage/helper/TreeProperties.h"
#include "terrainPlugin/meshgen/foliage/helper/FVectorShape.h"
#include <map>
#include "ETreeType.h"

/**
 * 
 */
class TERRAINPLUGIN_API MatrixTree
{
public:
	MatrixTree();
	~MatrixTree();

	void generate(ETerrainType type);

	MeshData &meshDataStemByReference();
	MeshData &meshDataLeafByReference();

private:
	void clean();
	void resetRotationMatrices();


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

	void wrapWithMesh(IndexChain &chainIn, MeshData &meshToAddTo);

	void createSubTrees(MMatrix &offset, TreeProperties &prop);
	void createSubTrees(MMatrix &offset, TreeProperties &propeties, int recursionLevel);
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

	std::vector<ETreeType> allTreeTypes();





	void generateLeafShapes();
	void generateLeafShape(ETreeType type);
	void generateStemShapes();
	void generateStemShapeFor(ETreeType type);

	std::map<ETreeType, std::vector<FVectorShape>> stemMap;
	std::map<ETreeType, FVectorShape> leafMap;

};
