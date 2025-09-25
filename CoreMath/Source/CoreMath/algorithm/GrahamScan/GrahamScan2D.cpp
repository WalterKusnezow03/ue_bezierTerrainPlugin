#include "GrahamScan2D.h"


GrahamScan2D::GrahamScan2D()
{
}

GrahamScan2D::~GrahamScan2D()
{
}



/// @brief will compute the convex hull on the XY pane
/// @param points points will be replaced with the convex hull of the passed points if possible
void GrahamScan2D::ComputeConvexHull(TArray<FVector2D> &points) {
    TArray<FVector2D> eliminated;
    ComputeConvexHull(points, eliminated, false); // dont look for eliminated
}

void GrahamScan2D::ComputeConvexHull(TArray<FVector2D> &points, TArray<FVector2D> &eliminated){
    ComputeConvexHull(points, eliminated, true); // dont look for eliminated
}


/// @brief checks if the point is clockwise rotated for the graham scan
bool GrahamScan2D::IsClockwise(const FVector2D& a, const FVector2D& b, const FVector2D& c) {
    return (b.X - a.X) * (c.Y - a.Y) - (b.Y - a.Y) * (c.X - a.X) < 0;
    // < 0 means, kolliniear vectors are kept and ignored.
}





/// @brief will compute the convex hull on the XY pane
/// @param points points will be replaced with the convex hull of the passed points if possible
void GrahamScan2D::ComputeConvexHull(
    TArray<FVector2D> &points, 
    TArray<FVector2D> &eliminated,
    bool findEliminated
) {
    TArray<FVector2D> convexHull; 

    if (points.Num() < 3) {
        return;
    }

    // Sort points to calculate the lower hull
    points.Sort([](const FVector2D& A, const FVector2D& B)
    {
        if (A.X == B.X)
        {
            return A.Y < B.Y;
        }
        return A.X < B.X;
    });




    // Calculate the lower hull
    for (const FVector2D& point : points) {
        while (convexHull.Num() >= 2 && !IsClockwise(convexHull[convexHull.Num() - 2], convexHull.Last(), point)) {
            convexHull.Pop();
        }
        convexHull.Add(point);
    }

    //Calculate the upper hull
    int lowerHullCount = convexHull.Num();
    for (int i = points.Num() - 2; i >= 0; --i) {
        FVector2D point = points[i];
        while (
            convexHull.Num() > lowerHullCount && 
            !IsClockwise(convexHull[convexHull.Num() - 2], convexHull.Last(), point)
        ) {
            convexHull.Pop();//den geprüften punkt poppen
        }
        convexHull.Add(point); //neuen punkt pushen
    }

    //Remove the last point which might be duplicated
    if (convexHull.Num() > 1) {
        FVector2D first = convexHull[0];
        FVector2D last = convexHull.Last();
        if(first == last){
            convexHull.Pop();   
        }
    }

    //identify eliminated points
    if(findEliminated){
        for (int i = 0; i < points.Num(); i++){
            FVector2D &current = points[i];
            bool found = false;
            for (int j = 0; j < convexHull.Num(); j++)
            {
                if(convexHull[j] == current){
                    found = true;
                    break;
                }
            }
            if(!found){
                eliminated.Add(current);
            }
        }
    }
    

    points = convexHull; //override points
}
