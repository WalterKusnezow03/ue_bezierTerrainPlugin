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

std::vector<int> &IndexChain::indexChain(){
    return indexChainVector;
}


void IndexChain::setOffsetMatrix(MMatrix &mat){
    offsetMatrix = mat;
}


MMatrix &IndexChain::offsetMatrixRef(){
    return offsetMatrix;
}