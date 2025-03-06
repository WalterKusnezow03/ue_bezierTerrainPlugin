// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "p2/entities/customIk/MMatrix.h"
#include "GrahamScan.h"

GrahamScan::GrahamScan()
{
}

GrahamScan::~GrahamScan()
{
}



/// @brief will compute the convex hull on the XY pane
/// @param points points will be replaced with the convex hull of the passed points if possible
void GrahamScan::ComputeConvexHull(std::vector<FVector> &points) {
    std::vector<FVector> eliminated;
    ComputeConvexHull(points, eliminated, false); // dont look for eliminated
}

/// @brief checks if the point is clockwise rotated for the graham scan
/// @param a 
/// @param b 
/// @param c 
/// @return 
bool GrahamScan::IsClockwise(const FVector& a, const FVector& b, const FVector& c) {
    return (b.X - a.X) * (c.Y - a.Y) - (b.Y - a.Y) * (c.X - a.X) < 0;
    // < 0 means, kolliniear vectors are kept and ignored.
}







/// @brief will compute the convex hull on any pane!
/// @param points points will be replaced with the convex hull of the passed points if possible
void GrahamScan::ComputeConvexHull(std::vector<FVector> &points, FVector paneNormal) {

    MMatrix lookUp;
    lookUp.pitchRadAdd(-90);
    MMatrix normalRotator = MMatrix::createRotatorFrom(paneNormal); // x is forward, caution, add pitch -90
    MMatrix finalMat = normalRotator * lookUp;
    

    MMatrix inverse = finalMat; 
    inverse.transpose(); // M = R, R^-1 == R^T

    for (int i = 0; i < points.size(); i++){
        points[i] = inverse * points[i];
    }

    ComputeConvexHull(points);

    for (int i = 0; i < points.size(); i++){
        points[i] = finalMat * points[i];
    }

}



//not tested
void GrahamScan::ComputeConvexHullAutoProjection(std::vector<FVector> &points){
    if(points.size() > 1){
        FVector front = points[0];
        FVector completeNormal;
        for (int i = 1; i < points.size(); i++){

            // only positive normals if set is unordered;
            FVector normal = FVector::CrossProduct(front, points[i]);
            if(normal.Z < 0.0f){
                normal *= -1.0f;
            }
            completeNormal += normal;
        }
        completeNormal = completeNormal.GetSafeNormal();
        ComputeConvexHull(points, completeNormal);
    }
}



void GrahamScan::ComputeConvexHullAutoProjection(
    std::vector<FVector> &points,
    std::vector<FVector> &eliminated
){
    if(points.size() > 1){
        FVector front = points[0];
        FVector completeNormal;
        for (int i = 1; i < points.size(); i++){

            // only positive normals if set is unordered;
            FVector normal = FVector::CrossProduct(front, points[i]);
            if(normal.Z < 0.0f){
                normal *= -1.0f;
            }
            completeNormal += normal;
        }
        completeNormal = completeNormal.GetSafeNormal();
        ComputeConvexHull(points, completeNormal, eliminated);
    }
}











/// @brief will compute the convex hull on any pane!
/// @param points points will be replaced with the convex hull of the passed points if possible
void GrahamScan::ComputeConvexHull(
    std::vector<FVector> &points, 
    FVector paneNormal,
    std::vector<FVector> &eliminated
) {

    MMatrix lookUp;
    lookUp.pitchRadAdd(-90);
    MMatrix normalRotator = MMatrix::createRotatorFrom(paneNormal); // x is forward, caution, add pitch -90
    MMatrix finalMat = normalRotator * lookUp;
    

    MMatrix inverse = finalMat; 
    inverse.transpose(); // M = R, R^-1 == R^T

    for (int i = 0; i < points.size(); i++){
        points[i] = inverse * points[i];
    }

    ComputeConvexHull(points, eliminated, true);

    for (int i = 0; i < points.size(); i++){
        points[i] = finalMat * points[i];
    }

}


/// @brief will compute the convex hull on the XY pane
/// @param points points will be replaced with the convex hull of the passed points if possible
void GrahamScan::ComputeConvexHull(
    std::vector<FVector> &points, 
    std::vector<FVector> &eliminated,
    bool findEliminated
) {
    std::vector<FVector> convexHull; 

    if (points.size() < 3) {
        return;
    }

    // Sort points to calculate the lower hull
    //Z is up and ignored here.
    std::sort(points.begin(), points.end(), [](const FVector& a, const FVector& b) {
        if (a.X == b.X) {
            return a.Y < b.Y;
        }
        return a.X < b.X;
    });

    // Calculate the lower hull
    for (const FVector& point : points) {
        while (convexHull.size() >= 2 && !IsClockwise(convexHull[convexHull.size() - 2], convexHull.back(), point)) {
            convexHull.pop_back();
        }
        convexHull.push_back(point);
    }

    //Calculate the upper hull
    int lowerHullCount = convexHull.size();
    for (int i = points.size() - 2; i >= 0; --i) {
        FVector point = points[i];
        while (
            convexHull.size() > lowerHullCount && 
            !IsClockwise(convexHull[convexHull.size() - 2], convexHull.back(), point)
        ) {
            convexHull.pop_back();//den geprüften punkt poppen
        }
        convexHull.push_back(point); //neuen punkt pushen
    }

    //Remove the last point which might be duplicated
    if (!convexHull.empty()) {
        convexHull.pop_back();
    }

    //identify eliminated points
    if(findEliminated){
        for (int i = 0; i < points.size(); i++){
            FVector &current = points[i];
            bool found = false;
            for (int j = 0; j < convexHull.size(); j++)
            {
                if(convexHull[j] == current){
                    found = true;
                    break;
                }
            }
            if(!found){
                eliminated.push_back(current);
            }
        }
    }
    

    points = convexHull;
}