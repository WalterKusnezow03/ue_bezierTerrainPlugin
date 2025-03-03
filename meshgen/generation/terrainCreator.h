// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "terrainHillSetup.h"
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
	static const int CHUNKSIZE = 10;
	static const int ONEMETER = 100; //one meter constant
	static const int fractureHeightStep = 3;

	static const int MAXHEIGHT = 15000; //3000 is a good value, dont change

	

	int chunkNum();
	
	void createTerrainAndSpawnMeshActors(UWorld *world, int meters);
	void setFlatArea(FVector &location, int sizeMetersX, int sizeMetersY);

	//apply terrain
	void applyTerrainDataToMeshActors(std::vector<AcustomMeshActor *> &actors);
	void applyTerrainDataToMeshActors(std::vector<AcustomMeshActorBase *> &actors);

	//raycast
	int getHeightFor(FVector &position);

	void plotAllChunks(UWorld *world);

	const int MINCHUNK_HILL = 5; //5x5 min hill size

private:
	static const int HEIGH_AVG_SNOWHILL_LOWERBOUND = 200000; //200 * 100cm

	void createTerrain(UWorld *world, int meters);


	class chunk{
		public:
			chunk(int xPos, int yPos);
			~chunk();

			int getHeightFor(FVector &a);
			FVector position();


			void addheightForAll(int value);
			void scaleheightForAll(float value);
			void setheightForAll(float value);

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

		private:
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

	class UWorld *worldPointer;

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
	void applyHillData(terrainHillSetup &hillData);





	//--- terrain type apply helpers ---
	void randomizeTerrainTypes(UWorld *world);
	void applyTerrainTypeBetween(FVector &a, FVector &b, ETerrainType typeIn);
	terrainCreator::chunk *chunkAt(int x, int y);

	void applySpecialTerrainTypesByHeight();
};
