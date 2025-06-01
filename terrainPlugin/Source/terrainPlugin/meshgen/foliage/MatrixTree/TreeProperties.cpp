// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeProperties.h"

TreeProperties::TreeProperties()
{
    leafcountPerJointSaved = 10;
    recursionLevelInternalMax = 0;
}

TreeProperties::TreeProperties(const TreeProperties &other){
    if(this != &other){
        *this = other;
    }
}

TreeProperties &TreeProperties::operator=(const TreeProperties &other){
    if(this == &other){
        return *this;
    }
    terrainTypes = other.terrainTypes;
    type = other.type;
    detailStepCm = other.detailStepCm;
    leafcountPerJointSaved = other.leafcountPerJointSaved;
    partsPerSubtreeSaved = other.partsPerSubtreeSaved;
    subTreeCountSaved = other.subTreeCountSaved;
    recursionLevelInternalMax = other.recursionLevelInternalMax;
    pitchRotationMax = other.pitchRotationMax;
    pitchRotationPerRecursion = other.pitchRotationPerRecursion;

    leafMaterial = other.leafMaterial;
	stemMaterial = other.stemMaterial;
    return *this;
}

TreeProperties::~TreeProperties()
{
}



TreeProperties::TreeProperties(
    int detailStepIn, 
    ETreeType typeIn, 
    ETerrainType terrainTypeIn,
    int leafcountPerJointIn,
    int partsPerSubtreeIn,
    int subTreeCountIn
){
    subTreeCountSaved = std::abs(subTreeCountIn);
    leafcountPerJointSaved = std::abs(leafcountPerJointIn);
    addTerrainType(terrainTypeIn);
    updateDetailStep(detailStepIn);

    type = typeIn;
    partsPerSubtreeSaved = std::abs(partsPerSubtreeIn);
}

void TreeProperties::updateDetailStep(int detailStepIn){
    detailStepIn = std::abs(detailStepIn);
    if(detailStepIn < 1){
        detailStepIn = 1;
    }
    detailStepCm = std::abs(detailStepIn);
}




/// @brief adds the terrain type to the vector if not contained yet
/// @param typeIn some terrain type
void TreeProperties::addTerrainType(ETerrainType typeIn){
    bool found = false;
    for (int i = 0; i < terrainTypes.size(); i++)
    {
        if(terrainTypes[i] == typeIn){
            found = true;
            i = terrainTypes.size();
        }
    }
    if(!found){
        terrainTypes.push_back(typeIn);
    }
}

/// @brief terrain types for this property preset, do not modify
/// @return terrainProperties
std::vector<ETerrainType> &TreeProperties::getTerrainTypes(){
    return terrainTypes;
}


/// @brief returns the detail step between matrices
/// @return 
int TreeProperties::getDetailStep(){
    return detailStepCm;
}


/// @brief returns the tree type of this preset
/// @return type of this tree
ETreeType TreeProperties::getTreeType(){
    return type;
}

/// @brief returns the lead count per (top) joint for this tree wanted
/// @return 
int TreeProperties::leafCountPerJoint(){
    return leafcountPerJointSaved;
}

/// @brief returns the parts per subtree to create (half to full count for better randomness of trees)
/// @return 
int TreeProperties::partsPerSubtree(){
    if(partsPerSubtreeSaved <= 4){
        return partsPerSubtreeSaved;
    }
    int half = partsPerSubtreeSaved / 2;
    int rand = std::abs(FVectorUtil::randomNumber(half, partsPerSubtreeSaved));
    return rand;
}


int TreeProperties::subTreeCount(){
    int rand = FVectorUtil::randomNumber(subTreeCountSaved / 2, subTreeCountSaved);
    if(rand != 1){
        return rand;
    }
    return 0;
}

void TreeProperties::setRecursionLevelMax(int count){
    recursionLevelInternalMax = std::abs(count);
    if(recursionLevelInternalMax < 1){
        recursionLevelInternalMax = 1;
    }
}

int TreeProperties::resursionLevelMax(){
    return recursionLevelInternalMax;
}

void TreeProperties::setTargetedMaterials(materialEnum stem, materialEnum leaf){
    stemMaterial = stem;
    leafMaterial = leaf;
}

materialEnum TreeProperties::targetMaterialForStem(){
    return stemMaterial;
}

materialEnum TreeProperties::targetMaterialForLeaf(){
    return leafMaterial;
}



/// @brief max rotation on any recursion level
/// @param degree 
void TreeProperties::setRotationMax(float degree){
    pitchRotationMax = std::abs(degree);

    pitchRotationPerRecursion = 0.0f;
    if (recursionLevelInternalMax > 0)
    {
        pitchRotationPerRecursion = pitchRotationMax / recursionLevelInternalMax;
    }
}

float TreeProperties::rotationOnRecursionLevel(int level){
    float absRange = std::abs(level * pitchRotationPerRecursion);
    absRange = std::min(180.0f, absRange);

    return FVectorUtil::randomFloatNumber(-absRange, absRange);
}