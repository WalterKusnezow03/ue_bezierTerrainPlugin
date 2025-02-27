// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ELod.h"
#include "MeshData.h"
#include <map>

/**
 * 
 */
class P2_API MeshDataLod
{
public:
	MeshDataLod();
	~MeshDataLod();

	void replace(ELod lodlevel, MeshData &meshdata);
	MeshData &meshDataReference(ELod lodLevel);

private:
	std::map<ELod, MeshData> lodLayerMap;
};
