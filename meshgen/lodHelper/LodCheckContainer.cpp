

#include "CoreMinimal.h"
#include "p2/meshgen/customMeshActorBase.h"
#include "p2/meshgen/ELod.h"
#include "LodCheckContainer.h"

LodCheckContainer::LodCheckContainer(){
    modifyUpperDistanceLimitFor(ELod::lodNear, 100 * 100);
    modifyUpperDistanceLimitFor(ELod::lodMiddle, 200 * 100);
    modifyUpperDistanceLimitFor(ELod::lodFar, 300 * 100);
}

LodCheckContainer::LodCheckContainer(FVector &a, FVector &b){
    modifyUpperDistanceLimitFor(ELod::lodNear, 100 * 100);
    modifyUpperDistanceLimitFor(ELod::lodMiddle, 200 * 100);
    modifyUpperDistanceLimitFor(ELod::lodFar, 300 * 100);

    checkLod(a, b);
}

LodCheckContainer::~LodCheckContainer(){

}

//public api

void LodCheckContainer::checkLod(FVector &a, FVector &b){
    thisLocation = a;
    playerLocation = b;
    wasLodMiddleEdgeCase = false;
    wasLodFarEdgeCase = false;

    checkLod();
}

void LodCheckContainer::modifyUpperDistanceLimitFor(ELod lod, int newMaxDistance){
    distances[lod] = std::abs(newMaxDistance);
}

ELod LodCheckContainer::lod(){
    return lodFound;
}

bool LodCheckContainer::hideActorByLod(){
    return hideActorFlag;
}

bool LodCheckContainer::lodWasEdgeCaseToELodFar(){
    return wasLodFarEdgeCase;
}

bool LodCheckContainer::lodWasEdgeCaseToELodMiddle(){
    return wasLodMiddleEdgeCase;
}


//private

void LodCheckContainer::checkLod(){
    bool edgeCase = false;
    lodFound = lodLevelByDistanceTo(edgeCase);
    if(lodFound == ELod::lodFar){
        hideActorFlag = true;
        wasLodFarEdgeCase = edgeCase;
    }
    if(lodFound == ELod::lodNear){
        wasLodMiddleEdgeCase = edgeCase;
    }
}

ELod LodCheckContainer::lodLevelByDistanceTo(bool &isEdgeCase){
    ELod outLod = ELod::lodFar;
    std::vector<ELod> vec = AcustomMeshActorBase::lodVector();

    if(vec.size() > 0){
        int smallestEdgeDistance = -1;
        for (int i = 0; i < vec.size(); i++)
        {
            if(isInRange(playerLocation, maxDistanceForLod(vec[i]), smallestEdgeDistance)){
                outLod = vec[i];
                break;
            }
        }
        if(smallestEdgeDistance <= edgeDistanceFrom){
            isEdgeCase = true;
        }
    }

    return outLod;
}



bool LodCheckContainer::isInRange(FVector &a, int maxDistance, int &smallestEdge){
    int distOnX = (maxDistance - std::abs(a.X - thisLocation.X));
    int distOnY = (maxDistance - std::abs(a.Y - thisLocation.Y));
    //int distOnZ = (maxDistance - std::abs(a.Z - thisLocation.Z));

    smallestEdge = distOnX < distOnY ? distOnX : distOnY;

    return distOnX > 0 && distOnY > 0;
    //&&distOnZ > 0;
}



///@brief returns the max distance for a given lod level
int LodCheckContainer::maxDistanceForLod(ELod lodLevel){
    if(distances.find(lodLevel) != distances.end()){
        return distances[lodLevel];
    }
    int oneMeter = 100;
    return oneMeter * 100; // 300
}
