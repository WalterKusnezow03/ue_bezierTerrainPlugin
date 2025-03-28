// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/meshgen/MeshData.h"

/**
 * 
 */
class P2_API roomBoundData
{
public:
	roomBoundData(int xin, int yin, int xscalein, int yscalein, int numberIn);
	int number = 0;

	~roomBoundData();

	void connect(roomBoundData &other);
	void addDoorPosition(int xIn, int yIn);
	void addWindowPosition(int xIn, int yIn);

	std::vector<FVector> relativeWindowPositionsCm(int onemeter);
	std::vector<FVector> relativeDoorPositionsCm(int onemeter);

	int xpos();
	int ypos();
	int xScale();
	int yScale();
	int xMax();
	int yMax();

	FVector positionInMeterSpace(int onemeter);

	void staircasePossibleBounds(FVector2D &start, FVector2D &end);
	void staircasePossbileBoundsInMeters(FVector2D &start, FVector2D &end, int onemeter);
	void staircasePossbileBoundsInMetersQuad(
		std::vector<FVector> &verteciesOut,
		int onemeter
	);

	void appendBottomOrTopClosed(
		MeshData &appendTo, 
		bool gapForStairs, 
		int oneMeter,
		float heightOffset
	);

	std::vector<FVector> &stairBoundsQuadInIndexSpaceRef(int onemeter);

	

private:
	bool stairBoundsFound = false;
	FVector2D startStairBounds;
	FVector2D endStairBounds;
	void findStairBounds(int scaleX, int scaleY);
	bool doorIndexIsValid(int index);
	bool exceedsBounds(FVector &other);
	std::vector<FVector> stairBoundsQuad;

	void processFoundStairBounds(
		std::vector<FVector> &ref,
		int onemeter
	);

	int x = 0;
	int y = 0;
	int xscale = 0;
	int yscale = 0;

	std::vector<FVector> doorPositions;
	std::vector<FVector> windowPositions;

	bool contains(std::vector<FVector> &other, int xin, int yin);
	bool contains(std::vector<FVector> &other, FVector pos);

	void clamp(FVector &other);

	void defineOverlap(
    	roomBoundData &other,
		int otherX,
		int otherY,
		int otherXmax,
		int otherYmax,
		int thisX,
		int thisY,
		int thisXmax,
		int thisYmax,
		bool xyflipped
	);
	bool oneApart(int a, int b, bool &flip);
	// std::vector<roomBoundData *> &order);

	std::vector<roomBoundData *> connectedNeighbors;
	bool hasNeighbor(roomBoundData *other);
};
