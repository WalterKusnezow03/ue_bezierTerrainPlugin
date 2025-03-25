// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "p2/rooms/testing/roomBoundData.h"
#include "p2/util/TTouple.h"
#include "CoreMinimal.h"

/**
 * 
 */
class P2_API layoutMaker
{
public:
	layoutMaker();
	~layoutMaker();

	void makeLayout(
		int xMeters,
		int yMeters,
		std::vector<TTouple<int, int>> &possibleSizes, 
		std::vector<roomBoundData> &output
	);

	

private:
	std::vector<std::vector<roomBoundData*>> pointerField;
	std::vector<roomBoundData *> owningRoomsVec;
	
	void clearAndFill(int x, int y);
	void clearField();

	bool fieldIsFilled();

	void tryFit(int xSize, int ySize);
	bool canFit(int fromX, int fromY, int xSize, int ySize);
	void fill(int fromX, int fromY, int xSize, int ySize, roomBoundData *room);

	void createDoorsAndWindows();
	void makeDoors();

	void makeWindows();
	bool isOuterEdge(int i);
	bool isOuterEdge(int i, int j);

	void checkFree(int xf, int yf, int xto, int yto, std::vector<FVector> &output);
	bool canBeWindow(int i, int j);
};
