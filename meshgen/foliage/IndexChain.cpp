// Fill out your copyright notice in the Description page of Project Settings.


#include "IndexChain.h"

IndexChain::IndexChain()
{
}

IndexChain::~IndexChain()
{
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