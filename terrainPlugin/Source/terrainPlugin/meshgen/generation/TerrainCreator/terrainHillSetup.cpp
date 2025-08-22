// Fill out your copyright notice in the Description page of Project Settings.


#include "terrainHillSetup.h"
#include "GameCore/util/FVectorUtil.h"

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
terrainHillSetup::terrainHillSetup(const terrainHillSetup &other){
    if(this != &other){
        *this = other;
    }
}
terrainHillSetup &terrainHillSetup::operator=(const terrainHillSetup &other){
    if(this == &other){
        return *this;
    }
    xPos = other.xPos;
	yPos = other.yPos;
    xTarget = other.xTarget;
    yTarget = other.yTarget;
    zMinheightAdd = other.zMinheightAdd;
    zMaxheightAdd = other.zMaxheightAdd;
    forceHeight = other.forceHeight;
    forceHeightWasSet = other.forceHeightWasSet;

    return *this;
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


//overlap check 

///@brief will return if a given area overlaps with this hill setup bounds
bool terrainHillSetup::doesOverlapArea(int startX, int startY, int scaleX, int scaleY){
    //can overlap from start
    //or overlap from other into start
    int targetXOther = startX + scaleX;
    int targetYOther = startY + scaleY;

    if(
        (overlapOnX(startX) || overlapOnX(targetXOther)) &&
        (overlapOnY(startY) || overlapOnY(targetYOther))
    ){
        return true;
    };

    return isEnclousingX(startX, targetXOther) && isEnclousingY(startY, targetYOther);
}


bool terrainHillSetup::overlapOnX(int checkvalue){
    return isInRange(xPosCopy(), xTargetCopy(), checkvalue);
}

bool terrainHillSetup::overlapOnY(int checkvalue){
    return isInRange(yPosCopy(), yTargetCopy(), checkvalue);
}

bool terrainHillSetup::isInRange(int from, int to, int checkValue){
    return checkValue >= from && checkValue <= to;
}

bool terrainHillSetup::isEnclousingX(int from, int to){
    return from <= xPosCopy() && to >= xTargetCopy();
}
bool terrainHillSetup::isEnclousingY(int from, int to){
    return from <= yPosCopy() && to >= yTargetCopy();
}



//extend bounds

///@brief will extend the bounds of the hill bounding box in every direction by
///an absolute size
void terrainHillSetup::extendInEveryDirectionBy(int count){
    count = std::abs(count);

    xPos -= count;
	yPos -= count;
    xTarget += count;
    yTarget += count;
}




FVector terrainHillSetup::center(){
    FVector centerVec(
        (xPos + xTarget) / 2.0f,
        (yPos + yTarget) / 2.0f,
        0.0f
    );
    return centerVec;
}