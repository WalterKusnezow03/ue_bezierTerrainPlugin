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
	~terrainHillSetup();

	int xPosCopy();
	int yPosCopy();
	int xTargetCopy();
	int yTargetCopy();

	int getRandomHeightFromRange();

private:
	int xPos = 0;
	int yPos = 0;
	int xTarget = 1;
	int yTarget = 1;
	int zMinheightAdd = 100;
	int zMaxheightAdd = 200;
};
