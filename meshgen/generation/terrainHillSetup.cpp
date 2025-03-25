// Fill out your copyright notice in the Description page of Project Settings.


#include "terrainHillSetup.h"

terrainHillSetup::terrainHillSetup(int posX, int posY, int scaleX, int scaleY, int minHeightAdd, int maxHeightAdd)
{
    xPos = posX;
	yPos = posY;
    xTarget = posX + scaleX;
    yTarget = posY + scaleY;
    zMinheightAdd = minHeightAdd;
	zMaxheightAdd = maxHeightAdd;

    forceHeightWasSet = false;
}

terrainHillSetup::~terrainHillSetup()
{
}

int terrainHillSetup::xPosCopy(){
    return xPos;
}
int terrainHillSetup::yPosCopy(){
    return yPos;
}
int terrainHillSetup::xTargetCopy(){
    return xTarget;
}
int terrainHillSetup::yTargetCopy(){
    return yTarget;
}



int terrainHillSetup::getHeightIfSetOrRandomHeight(){
    if(forceHeightWasSet){
        return forceHeight;
    }
    return FVectorUtil::randomNumber(zMinheightAdd, zMaxheightAdd);
}




void terrainHillSetup::forceSetHeight(int heightIn){
    forceHeight = heightIn;
    forceHeightWasSet = true;
}

int terrainHillSetup::getForcedSetHeight(){
    return forceHeight;
}