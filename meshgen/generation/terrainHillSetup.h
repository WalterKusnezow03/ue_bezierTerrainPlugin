// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * stores data for creating hills on the terrain, wraps values for better code readability
 */
class P2_API terrainHillSetup
{
public:
	terrainHillSetup(int posX, int posY, int scaleX, int scaleY, int minHeightAdd, int maxHeightAdd);
	terrainHillSetup(const terrainHillSetup &other);
	terrainHillSetup &operator=(const terrainHillSetup &other);

	~terrainHillSetup();

	int xPosCopy();
	int yPosCopy();
	int xTargetCopy();
	int yTargetCopy();

	int getHeightIfSetOrRandomHeight();

	void forceSetHeight(int heightIn); //forceHeight
	int getForcedSetHeight();

	bool doesOverlapArea(int startX, int startY, int scaleX, int scaleY);

	void extendInEveryDirectionBy(int count);

private:
	int xPos = 0;
	int yPos = 0;
	int xTarget = 1;
	int yTarget = 1;
	int zMinheightAdd = 100;
	int zMaxheightAdd = 200;

	int forceHeight = 0;
	bool forceHeightWasSet = false;

	bool overlapOnX(int checkvalue);
	bool overlapOnY(int checkvalue);
	bool isInRange(int from, int to, int checkValue);

	bool isEnclousingX(int from, int to);
	bool isEnclousingY(int from, int to);
};
