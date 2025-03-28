// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "terrainHillSetup.h"
#include <set>
#include "p2/meshgen/foliage/ETerrainType.h"
#include "p2/util/TVector.h"

/**
 * 
 */
class P2_API terrainCreator
{
public:
	terrainCreator();
	~terrainCreator();

	void debugCreateTerrain(UWorld *world);

	static const bool PLOTTING_ENABLED = false; // false;
	
	/// @brief saves the number of vertecies in one chunk (m^2)
	static const int CHUNKSIZE = 20; //10
	static const int ONEMETER = 100; //one meter constant
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
	int getHeightFor(FVector &position);

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

	static const int HEIGH_AVG_SNOWHILL_LOWERBOUND = 200000; //200 * 100cm
	static const int HEIGHT_MAX_OCEAN = 300; //10 * 100 meter

	void createTerrain(UWorld *world, int meters);
	void createTerrain(
		UWorld *world,
		int meters,
		std::vector<terrainHillSetup> &predefinedHillDataVecFlatArea // flat area
	);

	class chunk{
		public:
			chunk(int xPos, int yPos);
			~chunk();

			int getHeightFor(FVector &a);
			FVector position();
			FVector positionPivotBottomLeft();

			void addheightForAll(int value);
			void scaleheightForAll(float value);
			void setheightForAll(float value);
			void clampheightForAllUpperLimit(float value);
			void clampheightForAllUpperLimitByOwnAverageHeight();

			FVector2D getFirstXColumnAnchor(int xColumn);
			FVector2D getFirstYRowAnchor(int yRow);
			

			void plot(UWorld *world);

			void applyIndivualVertexIndexBased(
				int xIn,
				int yIn,
				float newHeight,
				bool override
			);

			std::vector<std::vector<FVector>> &readMap();

			std::vector<std::vector<FVector>> &readAndMerge(
				chunk *top,
				chunk *right,
				chunk *topRight
			);
			bool xIsValid(int a);
			bool yIsValid(int a);

			void setTreesBlocked(bool b);
			bool createTrees();

			ETerrainType getTerrainType();
			void updateTerraintype(ETerrainType typeIn);

			float heightAverage();
			float maxHeight();

			void setWasCreatedTrue();
			bool wasAlreadyCreated();

			void updateTerrainTypeBySpecialHeights();


		private:
			bool wasCreated = false;
			ETerrainType savedTerrainType = ETerrainType::ETropical;

			std::vector<std::vector<FVector>> innerMap;
			int x;
			int y;
			bool blockTrees = false;

			int clampInnerIndex(int a);
			
			

			int convertToInnerIndex(int value);
			int clampOuterYIndex(FVector2D &a);
			bool isInBounds(FVector &a);

			int xPositionInCm();
			int yPositionInCm();

			std::vector<FVector> readFirstXColumn();
			std::vector<FVector> readFirstYRow();
			FVector readBottomLeftCorner();
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
		std::set<terrainCreator::chunk *> &output);
};
