// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeProperties.h"

TreeProperties::TreeProperties()
{
    leafcountPerJointSaved = 10;
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
    terrainType = other.terrainType;
    type = other.type;
    height = other.height;
    detailStepCm = other.detailStepCm;
    leafcountPerJointSaved = other.leafcountPerJointSaved;
    partsPerSubtreeSaved = other.partsPerSubtreeSaved;
    subTreeCountSaved = other.subTreeCountSaved;
    return *this;
}

TreeProperties::~TreeProperties()
{
}



TreeProperties::TreeProperties(
    int heightIn, 
    int detailStepIn, 
    ETreeType typeIn, 
    ETerrainType terrainTypeIn,
    int leafcountPerJointIn,
    int partsPerSubtreeIn,
    int subTreeCountIn
){
    subTreeCountSaved = std::abs(subTreeCountIn);
    leafcountPerJointSaved = std::abs(leafcountPerJointIn);
    terrainType = terrainTypeIn;
    type = typeIn;
    height = std::abs(heightIn);
    detailStepCm = std::abs(detailStepIn);
    partsPerSubtreeSaved = std::abs(partsPerSubtreeIn);
}

int TreeProperties::getHeight(){
    return height;
}

int TreeProperties::getDetailStep(){
    return detailStepCm;
}

ETreeType TreeProperties::getTreeType(){
    return type;
}

ETerrainType TreeProperties::getTerrainType(){
    return terrainType;
}

int TreeProperties::leafCountPerJoint(){
    return leafcountPerJointSaved;
}

int TreeProperties::partsPerSubtree(){
    return partsPerSubtreeSaved;
}


int TreeProperties::subTreeCount(){
    int rand = FVectorUtil::randomNumber(0, subTreeCountSaved);
    if(rand != 1){
        return rand;
    }
    return 0;
}