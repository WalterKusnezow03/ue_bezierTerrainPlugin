// Fill out your copyright notice in the Description page of Project Settings.


#include "IndexChain.h"

IndexChain::IndexChain()
{
}

IndexChain::~IndexChain()
{
}

IndexChain::IndexChain(const IndexChain &other){
    if(this != &other){
        *this = other;
    }
}

IndexChain &IndexChain::operator=(const IndexChain &other){
    if(this == &other){
        return *this;
    }
    indexChainVector = other.indexChainVector;
    offsetMatrix = other.offsetMatrix;
    endMatrix = other.endMatrix;
    matrixChainVector = other.matrixChainVector;
    internalRecursionLevel = other.internalRecursionLevel;

    return *this;
}

void IndexChain::addIndex(int i){
    indexChainVector.push_back(i);
}

void IndexChain::addMatrix(MMatrix &other){
    matrixChainVector.push_back(other);
}

/// @brief resets the matrix chain and end mat but NOT the offsetmatrix!
void IndexChain::resetMatrices(){
    matrixChainVector.clear();
    endMatrix.makeIdentity();
}

std::vector<int> &IndexChain::indexChain(){
    return indexChainVector;
}
std::vector<MMatrix> &IndexChain::matrixChain(){
    return matrixChainVector;
}

void IndexChain::setOffsetMatrix(MMatrix &mat){
    offsetMatrix = mat;
}


MMatrix &IndexChain::offsetMatrixRef(){
    return offsetMatrix;
}

MMatrix &IndexChain::endMatrixRef(){
    return endMatrix;
}

void IndexChain::updateEndMatrix(MMatrix &other){
    endMatrix = other;
}



///@brief sets the internal recursion level for the scale matrix
void IndexChain::setRecursionLevel(int level){
    internalRecursionLevel = std::abs(level);
}

void IndexChain::setRecursionLevelAndPitch(int level, float pitch){
    setRecursionLevel(level);
    pitchOnRecursion = pitch;
}

int IndexChain::getRecursionLevel(){
    return internalRecursionLevel;
}


///@brief returns a value from [0,1] for a shape scale by the set recursion level
float IndexChain::scaleFractionByRecursionLevel(){
    int recursionLevel = getRecursionLevel();
    float scaleByRecursion = 1.0f;
    if(recursionLevel != 0){
        /*
        1 - (5 / 100) ----> zahl von 100% kann varieren.
        1 - (0.05)
        */
        float fullPercent = 50.0f;
        scaleByRecursion = 1.0f - (recursionLevel / fullPercent);
    }
    return scaleByRecursion;
}


MMatrix IndexChain::scaleXYAndRotationMatrixFromRecursionLevel(){
    MMatrix scaleMatrix;
    float scale = scaleFractionByRecursionLevel();
    scaleMatrix.scale(scale, scale, 1.0f);

    MMatrix rotation;
    rotation.pitchRadAdd(MMatrix::degToRadian(pitchOnRecursion));

    MMatrix outMatrix = rotation * scaleMatrix; //<-- lese richtung --
    return outMatrix;
}