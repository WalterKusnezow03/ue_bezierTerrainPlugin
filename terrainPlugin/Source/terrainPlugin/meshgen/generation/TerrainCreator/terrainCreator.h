// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "terrainHillSetup.h"
#include <set>
#include "terrainPlugin/meshgen/generation/helper/TerrainChunkSetup.h"
#include "GameCore/MeshGenBase/foliage/ETerrainType.h"
#include "GameCore/util/FVectorTouple.h"
#include "chunk.h"
#include "GameCore/util/TVector.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/RoadMaker.h"


#include "terrainPlugin/meshgen/generation/TerrainCreator/ChunkSetup/TerrainChunkMap.h"
#include "terrainPlugin/main/worldCache/ChunkParserMap.h"

/**
 * 
 */
class TERRAINPLUGIN_API terrainCreator
{
public:
	terrainCreator();
	~terrainCreator();

	//---- Use for terrain generation ----

	void createTerrainAndSetupChunkParserMap(
		TerrainChunkMap &heightMap, //chunk information (height and position in grid)
		ChunkParserMap &mapToFillDataTo //chunk parsermap to fill, which can setup mesh actors trough chunkparser
	);
	void applyTerrainDataIntoChunkParserMapCache(
		ChunkParserMap &mapToFillDataTo
	);




	
	static const bool PLOTTING_ENABLED = false; // false;
	

	int chunkNum();
	


	//raycast
	float getHeightFor(FVector &position);
	float getHeightFor(FVector2D &pos);
	void getHeightAndDistanceFromModVertex(
		FVector2D &a,
		float &height,
		float &distance
	);

	void plotAllChunks(UWorld *world);


	//create actors
	AcustomMeshActor *getNewMeshActor();



private:
	void applyTerrainDataIntoChunkParserAt(ChunkParserMap &mapToFillDataTo, int x, int y);


	

	class UWorld *worldPointer = nullptr;

	std::vector<std::vector<chunk>> map;


	void createArray(int chunks);
	void smooth3dMap();
	void smooth3dMap(FVector &a, FVector &b, int iterations);

	void applyColumnOrRow(
		int index,
		TVector<FVector2D> &data,
		bool isColumn
	);

	bool verifyIndex(int a);
	int clampIndex(int a);

public:
	//converts a cm value to a chunk index.
	int cmToChunkIndex(int a);

private:
	int cmToMeter(int a);
	int meterToInnerChunkIndex(int a);
	int cmToInnerChunkIndex(int a);




	//--- terrain type apply helpers ---
	void randomizeTerrainTypes();
	void randomizeTerrainTypes(UWorld *world);
	void applyTerrainTypeBetween(FVector &a, FVector &b, ETerrainType typeIn);
	
public:	
	chunk *chunkAt(int x, int y);
	chunk *chunkAt(terrainHillSetup &setup);
	TArray<chunk *> chunksAt(
		TArray<FVector> &positionsWorld
	);

private:
	std::vector<ETerrainType> createRandomTerrainTypes(int count);
	ETerrainType selectTerrainTypeExcluding(ETerrainType typeToExclude);

	void applySpecialTerrainTypesByHeight();


	void createRoads(UWorld* world);
	RoadMaker roadmaker;




	//NEW
	TerrainChunkMap setupMap;
	void setupFromChunkMapQuadPart(TerrainChunkMap &ref, int x, int y, int numChunksSide);
};
