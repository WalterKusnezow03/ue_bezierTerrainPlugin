#pragma once

#include "CoreMinimal.h"
#include "Edge.h"


class COREMATH_API DoubleEdge
{

public:
    DoubleEdge();
    ~DoubleEdge();

    DoubleEdge(FVector2D &v0, FVector2D &v1);
    

    bool DoesIntersectRedirect(DoubleEdge *other);

    bool Traversable();
    void Disassemble(TArray<FVector2D> &outArray, int maxIterations);

    //appends an edge to this next via e0 direction
    void AppendOtherToEdge0(DoubleEdge *other);

    FVector2D averageLocation();

private:
    bool directed = false;
    

    bool CanFinishByDistance(Edge *a, Edge *b);

    void Setup(FVector2D &v0, FVector2D &v1);

    Edge *edge0 = nullptr;
    Edge *edge1 = nullptr;

    bool EdgeValid();

    Edge *StartingNode();
};