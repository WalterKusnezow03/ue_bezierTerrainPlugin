// Fill out your copyright notice in the Description page of Project Settings.


#include "ParallellShapeMerger.h"
#include <algorithm>
#include "GrahamScan.h"
#include "p2/util/FVectorUtil.h"
#include <limits>

ParallellShapeMerger::ParallellShapeMerger()
{
}

ParallellShapeMerger::~ParallellShapeMerger()
{
}

/// expects the baseVectorIn to be in a sorted hull or line!
void ParallellShapeMerger::createParallelSortedShape(
    std::vector<FVector> &baseVectorIn,
    std::vector<FVector> &vectorToAlginAndSort
){
    if(baseVectorIn.size() <= 2 || vectorToAlginAndSort.size() <= 2){
        return;
    } 
    
    baseVector = baseVectorIn;
    vectorAligned = vectorToAlginAndSort;

    sortPointsByDistance(baseVector); //scheint zu funktionieren

    //sortieren nach erster richtung von base[0 udn 1]
    //da der vector jetzt in einer richtung sortiert ist
    sortPointsByDistance(vectorAligned, baseVector[0], baseVector[1]); 

    generateTriangleBuffer();
}


void ParallellShapeMerger::fillMissingAlignedVertecies(std::vector<FVector> &other){
    if(other.size() < baseVector.size()){

        int dist = (baseVector.size() - other.size());
        int missing = std::abs(dist);

        //append inbetween randoms
        int limit = other.size() - 2;
        for (int i = 0; i < missing; i++)
        {
            int index = FVectorUtil::randomNumber(0, limit) % limit;
            int next = index + 1;
            if (isInBounds<FVector>(other, index) && isInBounds<FVector>(other, next))
            {
                FVector middle = (other[index] + other[next]) / 2.0f;
                other.insert(other.begin() + next, middle);//einsetzen vor next
                limit++; //update limit after insert
            }
        }
    }
    
}






std::vector<FVector> &ParallellShapeMerger::baseVectorReference(){
    return baseVector;
}
std::vector<FVector> &ParallellShapeMerger::alignedVectorReference(){
    return vectorAligned;
}

///@brief will return a triangle buffer where the face look dir is not defined,
/// use "appendDoubleSided" in the meshdata class
std::vector<FVector> &ParallellShapeMerger::triangleBufferReference(){
    return triangleBuffer;
}





/**
 * 
 * generation for meshData
 * 
 */
void ParallellShapeMerger::generateTriangleBuffer(){
    for (int i = 1; i < baseVector.size(); i++){
        if(i < vectorAligned.size()){
            /*
            1->2
            |  |
            0<-3

            1 2
            0
              2
            0 3
            */
            FVector &v0 = baseVector[i - 1];
            FVector &v1 = baseVector[i];
            FVector &v2 = vectorAligned[i];
            FVector &v3 = vectorAligned[i - 1];

            triangleBuffer.push_back(v0);
            triangleBuffer.push_back(v1);
            triangleBuffer.push_back(v2);

            triangleBuffer.push_back(v0);
            triangleBuffer.push_back(v2);
            triangleBuffer.push_back(v3);
        }
    }
}





///@brief sorts a set of points beginning from points[0]
void ParallellShapeMerger::sortPointsByDistance(
    std::vector<FVector> &points
){
    if(points.size() <= 0){
        return;
    }
    sortPointsByDistance(
        points,
        points[0],
        points[0] //will ignore any direction
    );
}

///@brief sorts a point set in order of an specific direction 
/// (to align the shape in same clock direction to the other shape, to generate a proper non
/// overlapping triangle buffer)
///@param points set to sort
///@param startingPoint point to have the point set to start from sorting by distance
///@param nextToStartingPoint can be the same as starting point, or other to define a starting
/// direction to look for to have clockwise or counter clockwise sorting (in case of aligning 2 circles for
/// example)
void ParallellShapeMerger::sortPointsByDistance(
    std::vector<FVector> &points,
    FVector startingPoint,
    FVector nextToStartingPoint
){
    if(points.size() <= 0){
        return;
    }

    std::vector<FVector> output;
    std::vector<bool> wasPushed(points.size(), false);
    FVector prevPoint = startingPoint;

    //find starting point if was part of sorted array
    for (int i = 0; i < points.size(); i++){
        if(points[i] == startingPoint){
            wasPushed[i] = true;
            output.push_back(startingPoint);
            break;
        }
    }

    bool checkDir = false;
    FVector dir;
    if(startingPoint != nextToStartingPoint){
        dir = nextToStartingPoint - startingPoint; //AB = B - A
        checkDir = true;
    }

    //start sorting until output vector is full, then override 
    while(output.size() < points.size()){
        FVector found;

        if(output.size() == 1 && checkDir){
            //CHECK DIR ON START
            if(findClosestPointToAndMatchDirection(
                points,
                wasPushed,
                prevPoint,
                found,
                dir
            )){
                output.push_back(found);
                prevPoint = found;
                checkDir = false;
            }
        }else{
            //DEFAULT SEARCH
            if (findClosestPointTo(
                    points,
                    wasPushed,
                    prevPoint,
                    found
            )){
                output.push_back(found);
                prevPoint = found; 
            }
        }
    }
    //override
    points = output;
}

bool ParallellShapeMerger::findClosestPointTo(
    std::vector<FVector> &points,
    std::vector<bool> &usedIndices,
    FVector &closestPointSearchedFor,
    FVector &outpoint
){
    if(points.size() <= 0){
        return false;
    }

    int index = 0;
    FVector closestPoint;
    float closestDist = std::numeric_limits<float>::max();

    for (int i = 1; i < points.size(); i++)
    {
        if(usedIndices[i] == false){
            FVector &currentPoint = points[i];
            float newdist = FVector::Dist(currentPoint, closestPointSearchedFor);
            if(newdist < closestDist){
                closestDist = newdist;
                closestPoint = currentPoint;
                index = i;
            }
        }
    }
    usedIndices[index] = true; //copy out
    outpoint = closestPoint; //copy out
    return true;
}




bool ParallellShapeMerger::findClosestPointToAndMatchDirection(
    std::vector<FVector> &points,
    std::vector<bool> &usedIndices,
    FVector &closestPointSearchedFor,
    FVector &outpoint,
    FVector &directionToMatch
){
    if(points.size() <= 0){
        return false;
    }
    directionToMatch = directionToMatch.GetSafeNormal();

    int index = 0;
    FVector closestPoint;
    float closestDist = std::numeric_limits<float>::max();

    for (int i = 1; i < points.size(); i++)
    {
        if(usedIndices[i] == false){
            FVector &currentPoint = points[i];
            FVector connectCompare = currentPoint - closestPointSearchedFor; //AB = B - A
            connectCompare = connectCompare.GetSafeNormal();
            float dot = FVector::DotProduct(
                directionToMatch,
                connectCompare
            );
            if(dot > 0.0f){
                float newdist = FVector::Dist(currentPoint, closestPointSearchedFor);
                if(newdist < closestDist){
                    closestDist = newdist;
                    closestPoint = currentPoint;
                    index = i;
                }
            }

            
        }
    }
    usedIndices[index] = true; //copy out
    outpoint = closestPoint; //copy out
    return true;
}