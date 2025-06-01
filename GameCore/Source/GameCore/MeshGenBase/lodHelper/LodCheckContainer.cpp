

#include "CoreMinimal.h"
#include "GameCore/MeshGenBase/customMeshActorBase.h"
#include "GameCore/MeshGenBase/ELod.h"
#include "LodCheckContainer.h"

LodCheckContainer::LodCheckContainer(){
    int metersLodNear = 50;
    int metersLodMiddle = 200;
    int metersLodFar = 300;

    modifyUpperDistanceLimitFor(ELod::lodNear, metersLodNear * 100);
    modifyUpperDistanceLimitFor(ELod::lodMiddle, metersLodMiddle * 100);
    modifyUpperDistanceLimitFor(ELod::lodFar, metersLodFar * 100);
}

LodCheckContainer::LodCheckContainer(FVector &a, FVector &b) : LodCheckContainer(){
    //modifyUpperDistanceLimitFor(ELod::lodNear, 100 * 100);
    //modifyUpperDistanceLimitFor(ELod::lodFar, 300 * 100);

    checkLod(a, b);
}

LodCheckContainer::~LodCheckContainer(){

}

//public api

void LodCheckContainer::checkLod(FVector &a, FVector &b){
    thisLocation = a;
    playerLocation = b;
    lodWasEdgeCase = false;

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

bool LodCheckContainer::lodWasEdgeCaseToNextLod(){
    return lodWasEdgeCase;
}



//private

void LodCheckContainer::checkLod(){
    bool edgeCase = false;
    lodFound = lodLevelByDistanceTo(edgeCase);
    if(lodFound == ELod::lodFar){
        hideActorFlag = true;
    }

    lodWasEdgeCase = edgeCase;
}

ELod LodCheckContainer::lodLevelByDistanceTo(bool &isEdgeCase){
    ELod outLod = ELod::lodFar;
    std::vector<ELod> vec = AcustomMeshActorBase::lodVector();

    if(vec.size() > 0){
        int smallestEdgeDistance = -1;
        for (int i = 0; i < vec.size(); i++)
        {
            if(isInRange(maxDistanceForLod(vec[i]), isEdgeCase)){
                outLod = vec[i];

                break;
            }
        }
    }

    return outLod;
}



bool LodCheckContainer::isInRange(int maxDistance, bool &isEdgeCase){
    
    int distOnX = std::abs(playerLocation.X - thisLocation.X);
    int distOnY = std::abs(playerLocation.Y - thisLocation.Y);

    bool isInRangeCurrent = (distOnX < maxDistance) && (distOnY < maxDistance);
    if(isInRangeCurrent){
        int edgeDistance = distOnX > distOnY ? distOnX : distOnY;
        if(edgeDistance > maxDistance * 0.9f){ //in top 90 percent
            isEdgeCase = true;
        }
    }
    
    return isInRangeCurrent;
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
