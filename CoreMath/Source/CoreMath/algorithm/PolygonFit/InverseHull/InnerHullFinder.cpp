#include "InnerHullFinder.h"
#include "CoreMath/algorithm/GrahamScan/GrahamScan2D.h"

InnerHullFinder::InnerHullFinder(){

}

InnerHullFinder::~InnerHullFinder(){

}



void InnerHullFinder::EliminateOuterHull(TArray<FVector2D> &shape){
    EliminateOuterHull(shape, 1);
}

void InnerHullFinder::EliminateOuterHull(TArray<FVector2D> &shape, int iterations){
    //solange der vertex count der hülle, eliminated points >= 3 ist,
    //ist es eine valide most inner hull.

    //solange die convexe hülle abtragen bis das kleine polygon was valide ist
    //convex überig bleibt.

    GrahamScan2D scanner;
    for (int i = 0; i < iterations; i++){
        TArray<FVector2D> potentialNewInner;
        scanner.ComputeConvexHull(shape, potentialNewInner);
        if(potentialNewInner.Num() >= 3){
            shape = potentialNewInner;
        }else{
            return; //finished, no smaller shape found.
        }
    }
}


void InnerHullFinder::MakeMostInnerHull(TArray<FVector2D> &shape){
    EliminateOuterHull(shape, 100);
}