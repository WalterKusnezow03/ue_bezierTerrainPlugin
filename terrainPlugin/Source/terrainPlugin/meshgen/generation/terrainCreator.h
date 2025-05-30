// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "terrainHillSetup.h"
#include <set>
#include "terrainPlugin/meshgen/generation/helper/TerrainChunkSetup.h"
#include "GameCore/MeshGenBase/foliage/ETerrainType.h"
#include "GameCore/util/FVectorTouple.h"
#include "terrainPlugin/meshgen/generation/helper/HeightExtractionData.h"
#include "GameCore/util/TVector.h"

/**
 * 
 */
class TERRAINPLUGIN_API terrainCreator
{
public:
	terrainCreator();
	~terrainCreator();

	void debugCreateTerrain(UWorld *world);
	void debugCreateTerrain(UWorld *world, int meters);

	static const bool PLOTTING_ENABLED = false; // false;
	
	/// @brief saves the number of vertecies in one chunk (m^2)
	static const int CHUNKSIZE = 20; //10 //20
	static const int ONEMETER = 200; //one meter constant
	static const int fractureHeightStep = 3;

	static const int MAXHEIGHT = 15000; //3000 is a good value, dont change

	void Tick(FVector &playerLocation);

	int chunkNum();
	
	void createTerrainAndSpawnMeshActors(UWorld *world, int meters);
	void createTerrainAndCreateBuildings(
		UWorld *world, int meters
	);
	

	//apply terrain
	void applyTerrainDataToMeshActors();

	//raycast
	float getHeightFor(FVector &position);
	float getHeightFor(FVector2D &pos);
	void getHeightAndDistanceFromModVertex(
		FVector2D &a,
		float &height,
		float &distance
	);

	void plotAllChunks(UWorld *world);

	const int MINCHUNK_HILL = 5; //5x5 min hill size

	const int CHUNKSTOCREATEATONCE = 10;

private:
	void setFlatArea(FVector &location, int sizeMetersX, int sizeMetersY);

	void applyTerrainDataToMeshActors(
		int lowerX,
		int xLimit,
		int lowerY,
		int yLimit
	);
	void createWaterPaneAt(FVector &location);

	void markCreateOutpostsAt(
		std::vector<terrainHillSetup> &predefinedHillDataVecFlatArea
	);

	static const int HEIGH_AVG_SNOWHILL_LOWERBOUND = 200000; //200 * 100cm
	static const int HEIGHT_MAX_OCEAN = 200; 

	void createTerrain(UWorld *world, int meters);
	void createTerrain(
		UWorld *world,
		int meters,
		std::vector<terrainHillSetup> &predefinedHillDataVecFlatArea // flat area
	);

	

	class chunk
	{
	public:
		chunk(int xPos, int yPos);
		~chunk();

		TerrainChunkSetup makeSetupPackage();
		TerrainChunkSetup makeSetupPackage(
			chunk *top,
			chunk *right,
			chunk *topRight);

		float getHeightFor(FVector &a);
		void getHeightWith(
			HeightExtractionData &data
		);
		FVector position();
		FVector positionPivotBottomLeft();

		void addheightForAll(int value);
		void scaleheightForAll(float value);
		void setheightForAll(float value);
		void setheightForAllToAverage();
		void clampheightForAllUpperLimit(float value);
		void clampheightForAllUpperLimitByOwnAverageHeight();

		FVector2D getFirstXColumnAnchor(int xColumn);
		FVector2D getFirstYRowAnchor(int yRow);

		void plot(UWorld *world);

		void applyIndivualVertexIndexBased(
			int xIn,
			int yIn,
			float newHeight,
			bool override);

		std::vector<std::vector<FVector>> &readMap();

		std::vector<std::vector<FVector>> &readAndMerge(
			chunk *top,
			chunk *right,
			chunk *topRight);
		bool xIsValid(int a);
		bool yIsValid(int a);

		void setTreesBlocked(bool b);
		bool createTrees();

		ETerrainType getTerrainType();
		void updateTerraintype(ETerrainType typeIn);

		float heightAverage();
		float maxHeight();
		float minHeight();

		void setWasCreatedTrue();
		bool wasAlreadyCreated();

		void updateTerrainTypeBySpecialHeights();

		void markCreateOutpostTrue();

		void blockAreaForFoliage(
			FVector &v0,
			FVector &v1,
			FVector &v2,
			FVector &v3
		);
		void blockAreaForFoliage(FVector &a, FVector &b);
		void freePositionsForFoliageLocal(
			TArray<FVectorTouple> &outpositions);

	private:
		void generateBoundingIndicesFromWorldSpace(
			FVector &a,
			FVector &b,
			int &minX,
			int &minY,
			int &maxX,
			int &maxY
		);

		bool indexFreeForFoliage(int i, int j);
		void lockPositionForAnyFoliage(int i, int j);
		void lockPositionForAnyFoliageIfTrue(int i, int j, bool flag);

		bool wasCreated = false;
		ETerrainType savedTerrainType = ETerrainType::ETropical;
		bool createOutpost = false;

		std::vector<std::vector<FVector>> innerMap;
		std::vector<std::vector<bool>> innerMapFreePositions;
		int x;
		int y;
		bool blockTrees = false;

		int clampInnerIndex(int a);

		FVector normalFor(int i, int j);

		int convertToInnerIndex(int value);
		int clampOuterYIndex(FVector2D &a);
		bool isInBounds(FVector &a);

		int xPositionInCm();
		int yPositionInCm();

		void convertPositionToInnerIndexClamped(
			FVector inpos,
			int &i,
			int &j);

		std::vector<FVector> readFirstXColumn();
		std::vector<FVector> readFirstYRow();
		FVector readBottomLeftCorner();

		std::vector<bool> readFirstXColumnFoliage();
		std::vector<bool> readFirstYRowFoliage();
		bool readBottomLeftCornerFoliage();
	};

	class UWorld *worldPointer = nullptr;

	std::vector<std::vector<terrainCreator::chunk>> map;

	
	
	
	
	void smooth3dMap();
	void smooth3dMap(FVector &a, FVector &b, int iterations);

	void applyColumnOrRow(
		int index,
		TVector<FVector2D> &data,
		bool isColumn
	);

	bool verifyIndex(int a);
	int clampIndex(int a);

	int cmToChunkIndex(int a);
	int cmToMeter(int a);
	int meterToInnerChunkIndex(int a);
	int cmToInnerChunkIndex(int a);


	void scaleHeightForAll(float scale);

	//new chunk bezier curve
	void createRandomHeightMapChunkWide(int layers);

	terrainHillSetup createRandomHillData();
	terrainHillSetup createRandomHillData(int sizeX, int sizeY);
	void applyHillData(terrainHillSetup &hillData);
	void applyHillData(std::vector<terrainHillSetup> &hillDataVec);

	void flattenChunksForHillData(std::vector<terrainHillSetup> &hillDataVec);
	void flattenChunksForHillData(terrainHillSetup &hillData);

	void createChunkAtIfNotCreatedYet(int x, int y);

	//--- terrain type apply helpers ---
	void randomizeTerrainTypes(UWorld *world);
	void applyTerrainTypeBetween(FVector &a, FVector &b, ETerrainType typeIn);
	terrainCreator::chunk *chunkAt(int x, int y);
	terrainCreator::chunk *chunkAt(terrainHillSetup &setup);
	TArray<terrainCreator::chunk *> chunksAt(
		TArray<FVector> &positionsWorld
	);

	std::vector<ETerrainType> createRandomTerrainTypes(int count);
	ETerrainType selectTerrainTypeExcluding(ETerrainType typeToExclude);

	void applySpecialTerrainTypesByHeight();


	//--- flat outpost helper ---
	void createFlatAreas(
		int count,
		int minsizeChunks,
		int maxsizeChunks,
		int chunkRange,
		std::vector<terrainHillSetup> &output
	);
	void createFlatArea(
		int minsizeChunks,
		int maxsizeChunks,
		int chunkRange,
		std::vector<terrainHillSetup> &output
	);

	void findChunksEnclosedBy(
		std::vector<terrainHillSetup> &hills,
		std::vector<terrainCreator::chunk *> &output
	);
	void findChunksEnclosedBy(
		terrainHillSetup &hillData,
		std::set<terrainCreator::chunk *> &output
	);



	//--road--
	void createRoads(UWorld *world);
	void createRoads(MeshData &meshdata, int count);
	void createRoad(MeshData &meshdata);
	void processRoad(
		TVector<FVector2D> &curve,
		float roadWidth,
		MeshData &data,
		float _einheitsValue
	);
	FVector make3D(FVector2D &pos, float height);
	void lockQuadsFromParalellArrayLines(
		TArray<FVector> &line0,
		TArray<FVector> &line1
	);


	//create actors
	AcustomMeshActor *getNewMeshActor();
};
