// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshDataLod.h"
#include "MeshData.h"

MeshDataLod::MeshDataLod()
{
}

MeshDataLod::~MeshDataLod()
{
}

void MeshDataLod::replace(ELod lodLevel, MeshData &meshdata){
    lodLayerMap[lodLevel] = meshdata;
}

MeshData &MeshDataLod::meshDataReference(ELod lodLevel){
    if(lodLayerMap.find(lodLevel) == lodLayerMap.end()){
        MeshData newData;
        lodLayerMap[lodLevel] = newData;
    }
    return lodLayerMap[lodLevel];
}