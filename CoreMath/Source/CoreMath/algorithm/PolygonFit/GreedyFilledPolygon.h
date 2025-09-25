#pragma once 

#include "CoreMinimal.h"
#include "CoreMath/algorithm/PolygonFit/polygon/MPolygon.h"
#include "CoreMath/Matrix/2D/MMatrix2D.h"

/// ---- BROKEN -----
///@brief will fit a set of polygons into a dynamic polygon, given by an outline
///cannot make sure all of them are fit.
///-> will move along the outline and try to fit a polygon at some outline point.
/// (polygon is transformed to position and rotated until can fit with a fixed tries number.)
class COREMATH_API GreedyFilledPolygon : public MPolygon {
private:
    bool PushInwardsEdgesAndChangeShape = true; //false works BAD

public:
    GreedyFilledPolygon();
    ~GreedyFilledPolygon();

    void Add(TArray<MPolygon> &array);

private:
    int AddAndReturnNextIndex(MPolygon &p, int i);

    FVector2D poslatestClosestPos;

public:


    int PolygonCount();

    ///@brief copies all polygons, including this one, is first in array.
    void AppendAllPolygonsIncludingSelfIntoArray(TArray<MPolygon> &outArray);

    virtual bool DoesIntersect(const MPolygon &shapeIn) override;



private:
    bool SkipNeeded(
        int prevIndex,
        int resultIndex,
        int &sameIndexCount,
        int maxSameIndex
    );

    bool ShrinkNeeded(
        int prevIndex,
        int resultIndex
    );

    int FindClosestIndexToTopLeft(MPolygon &addedPolygon);
    int FindClosestIndexTo(const FVector2D &target);

    MMatrix2D MakeTransform(FVector2D &startingPivot, FVector2D &current);
    bool IndexIsValid(int i, TArray<MPolygon> &polygons);

    ///@brief can only be used if shapeTransformed.Num() > 0 !
    int ValidateIndex(int index);


    void PrintOutline();
    void ShrinkShapeFromRightOffVertecies(int addedCount, int reachedUntilIndex);

    FVector2D latestPosition;

    //might not be needed, in out param for append
    TArray<MPolygon> internalPolygonsAdded;

};