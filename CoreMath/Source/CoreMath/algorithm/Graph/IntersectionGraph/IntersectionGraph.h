#pragma once

#include "CoreMinimal.h"
#include "CoreMath/algorithm/Graph/IntersectionGraph/DoubleEdge.h"

class COREMATH_API IntersectionGraph {

public:
    IntersectionGraph();
    ~IntersectionGraph();

    void AddEdgesFromConnectedArray(TArray<FVector2D> &interpolated);
    void AddEdgesFromConnectedArray(TArray<FVector2D> &interpolated, int reduceDeail);

    

    void CreatePolygons(TArray<TArray<FVector2D>> &traversedOutPolygons);

private:
    TArray<DoubleEdge *> edges;
    TArray<DoubleEdge *> endPoints;

    bool AddEdge(DoubleEdge *edge, int limit); //Limit may be removed, just for testing to prevent self hittest
    bool AddAll(TArray<DoubleEdge *> &heapArray);

    bool IsValidPolygon(TArray<FVector2D> &array);
    bool IsValidPolygon(TArray<FVector2D> &array, int epsilon);

    DoubleEdge *findClosest(DoubleEdge *other);

    int countIntersections = 0;

    void RemoveDeadEdges();
};