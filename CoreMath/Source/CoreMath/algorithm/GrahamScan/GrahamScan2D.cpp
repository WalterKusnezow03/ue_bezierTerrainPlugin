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
bool GrahamScan2D::IsClockwise(
    const FVector2D& a, 
    const FVector2D& b, 
    const FVector2D& c
){
    return (b.X - a.X) * (c.Y - a.Y) - (b.Y - a.Y) * (c.X - a.X) < 0;
    // < 0 means, kolliniear vectors are kept and ignored.
}


/// @brief checks if the point is clockwise rotated for the graham scan
bool GrahamScan2D::IsClockwise(
    const TIndexedElement<FVector2D>& a, 
    const TIndexedElement<FVector2D>& b, 
    const TIndexedElement<FVector2D>& c
) {
    return IsClockwise(
        a.Get(),
        b.Get(),
        c.Get()
    );
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







///@brief makes a convex hull from the given points, array not modifed but index array out
///put modified: indices are made from points given, convex hull.
void GrahamScan2D::ComputeConvexHull(
    const TArray<FVector2D> &points, 
    TArray<int> &outIndices
){
    if (points.Num() < 3) {
        return;
    }

    TArray<TIndexedElement<FVector2D>> convexHull; 

    TArray<TIndexedElement<FVector2D>> pointsConverted;
    pointsConverted.SetNumUninitialized(points.Num());
    for (int i = 0; i < points.Num(); i++){
        pointsConverted[i] = TIndexedElement<FVector2D>(points[i], i);
    }



    // Sort points to calculate the lower hull
    pointsConverted.Sort([](
        const TIndexedElement<FVector2D>& A, 
        const TIndexedElement<FVector2D>& B
    )
    {
        const FVector2D &aVec = A.Get();
        const FVector2D &bVec = B.Get();

        if (aVec.X == bVec.X)
        {
            return aVec.Y < bVec.Y;
        }
        return aVec.X < bVec.X;
    });




    // Calculate the lower hull
    for (const TIndexedElement<FVector2D>& point : pointsConverted) {
        while (convexHull.Num() >= 2 && !IsClockwise(convexHull[convexHull.Num() - 2], convexHull.Last(), point)) {
            convexHull.Pop();
        }
        convexHull.Add(point);
    }

    //Calculate the upper hull
    int lowerHullCount = convexHull.Num();
    for (int i = pointsConverted.Num() - 2; i >= 0; --i) {
        TIndexedElement<FVector2D> &point = pointsConverted[i];
        while (
            convexHull.Num() > lowerHullCount &&
            !IsClockwise(convexHull[convexHull.Num() - 2], convexHull.Last(), point))
        {
            convexHull.Pop();//den geprüften punkt poppen
        }
        convexHull.Add(point); //neuen punkt pushen
    }

    //Remove the last point which might be duplicated
    if (convexHull.Num() > 1) {
        TIndexedElement<FVector2D> first = convexHull[0];
        TIndexedElement<FVector2D> last = convexHull.Last();
        if(first.Get() == last.Get()){ //compare positions
            convexHull.Pop();   
        }
    }



    outIndices.SetNum(convexHull.Num());
    for (int i = 0; i < convexHull.Num(); i++){
        TIndexedElement<FVector2D> &current = convexHull[i];
        outIndices[i] = current.GetIndex();
    }

    //points = convexHull; // override points
}