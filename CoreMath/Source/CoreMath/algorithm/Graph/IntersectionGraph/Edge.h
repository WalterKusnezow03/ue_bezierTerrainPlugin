#pragma once

#include "CoreMinimal.h"

class COREMATH_API Edge {

private:
    bool bLeftOff = false;

public:
    Edge();
    ~Edge();


    Edge(FVector2D &v0In, FVector2D &v1In);

    Edge(const Edge &other);
    Edge &operator=(const Edge &other);

   
    void SetNext(Edge *other);
    void SetNextSafe(Edge *other);

    void SetNextNullptr();

    Edge *GetNext();
    

   
    bool DoesIntersect(Edge *other);

    ///@brief returns if the end point of other edge is right off this edge
    bool IsRightOff(Edge *other);

    ///@brief returns if the end point of the other edge is right off this edge
    ///and further right back than the current next
    bool IsRightOffForReplacement(Edge *other);


    //appends v1 / end of edge into array
    void AppendTarget(TArray<FVector2D> &array);

    bool InRange(Edge *other);


    FVector2D location(){
        return (v0 + v1) / 2.0f;
    }

private:
    bool DoesIntersect(
        const FVector2D &v0In, 
        const FVector2D &v1In,
        const FVector2D &d0,
        const FVector2D &d1,
        FVector &IntersectionPoint
    );

    bool RightOffTest(FVector2D &a0, FVector2D &a1, FVector2D &check);

    
    Edge *next = nullptr;

    FVector2D v0;
    FVector2D v1;
};