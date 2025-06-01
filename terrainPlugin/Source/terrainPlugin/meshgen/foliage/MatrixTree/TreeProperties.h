// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ETreeType.h"
#include "AssetPlugin/gamestart/assetEnums/materialEnum.h"
#include "GameCore/MeshGenBase/foliage/ETerrainType.h"

/**
 * 
 */
class TERRAINPLUGIN_API TreeProperties
{
public:
	TreeProperties();
	TreeProperties(const TreeProperties &other);
	TreeProperties(
		int detailstep, 
		ETreeType typeIn, 
		ETerrainType terrainTypeIn,
		int leafcountPerJointIn,
		int partsPerSubtreeIn,
		int subTreeCountIn
	);

	TreeProperties &operator=(const TreeProperties &other);

	~TreeProperties();

	int getDetailStep();
	ETreeType getTreeType();
	

	void addTerrainType(ETerrainType typeIn);
	std::vector<ETerrainType> &getTerrainTypes();

	int leafCountPerJoint();
	int partsPerSubtree();

	int subTreeCount();

	void setRotationMax(float degree);
	float rotationOnRecursionLevel(int level);

	void setRecursionLevelMax(int count);
	int resursionLevelMax();

	void setTargetedMaterials(materialEnum stem, materialEnum leaf);
	materialEnum targetMaterialForStem();
	materialEnum targetMaterialForLeaf();

private:
	materialEnum leafMaterial = materialEnum::palmLeafMaterial;
	materialEnum stemMaterial = materialEnum::treeMaterial;

	
	int detailStepCm;
	ETreeType type;

	std::vector<ETerrainType> terrainTypes;

	int leafcountPerJointSaved;
	int subTreeCountSaved;

	int partsPerSubtreeSaved;

	void updateDetailStep(int detailStepIn);

	//rotation on recursion
	float pitchRotationMax = 0.0f;
	float pitchRotationPerRecursion = 0.0f;

	//new: recursion level maximum
	int recursionLevelInternalMax = 0;
};
