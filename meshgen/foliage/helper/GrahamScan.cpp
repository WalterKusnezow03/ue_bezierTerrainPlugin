// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
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

    // Calculate the upper hull
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

    points = convexHull;
}

/// @brief checks if the point is clockwise rotated for the graham scan
/// @param a 
/// @param b 
/// @param c 
/// @return 
bool GrahamScan::IsClockwise(const FVector& a, const FVector& b, const FVector& c) {
    //return (b.X - a.X) * (c.Y - a.Y) - (b.Y - a.Y) * (c.Y - a.Y) < 0;
    return (b.X - a.X) * (c.Y - a.Y) - (b.Y - a.Y) * (c.X - a.X) < 0;
    // < 0 means, kolliniear vectors are kept and ignored.
}
