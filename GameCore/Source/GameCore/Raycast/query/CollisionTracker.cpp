#include "CollisionTracker.h"
#include "GameCore/DebugHelper.h"

CollisionTracker::CollisionTracker(){

}

CollisionTracker::~CollisionTracker(){
    Clear();
}

void CollisionTracker::Clear(){
    tracked.Empty();
}

void CollisionTracker::AddIgnoredActor(AActor *actor){
    if(actor){
        if(!tracked.Contains(actor)){
            tracked.Add(actor);
            //DebugHelper::logMessage("CollisionTracker tracked added actor ", tracked.Num());
        }
    }
}

void CollisionTracker::RemoveIgnoredActor(AActor *actor){
    if(actor){
        if(tracked.Num() > 0 && tracked.Contains(actor)){
            int foundAtIndex = -1;
            //swap with back
            for (int i = 0; i < tracked.Num(); i++){
                AActor *current = tracked[i];
                if(current == actor){
                    foundAtIndex = i;
                    break;
                }
            }

            //swap to avoid array shift.
            if(foundAtIndex != -1){
                tracked[foundAtIndex] = tracked[tracked.Num() - 1]; //swap
                tracked.Pop(); //pop back
            }
        }
    }
}



/// @brief params with tracked actors, bTraceComplex false
/// @return 
FCollisionQueryParams CollisionTracker::getCollisonParams(){
    return makeParams(tracked);
}

///@brief gets collision params merged with another params object
FCollisionQueryParams CollisionTracker::getMergedCollisionParams(const CollisionTracker &other){
    TArray<AActor *> merged = MakeMergedArray(other);
    return makeParams(merged);
}

TArray<AActor *> CollisionTracker::MakeMergedArray(const CollisionTracker &other){
    TArray<AActor *> copy = tracked;
    MergeInto(copy, other);
    return copy;
}

void CollisionTracker::MergeInto(TArray<AActor *> &appendTo, const CollisionTracker &other){
    MergeInto(appendTo, other.tracked);
}

void CollisionTracker::MergeInto(TArray<AActor *> &appendTo, const TArray<AActor *> &other){
    for (int i = 0; i < other.Num(); i++){
        AActor *current = other[i];
        if(current && !appendTo.Contains(current)){
            appendTo.Add(current);
        }
    }
}

FCollisionQueryParams CollisionTracker::makeParams(const TArray<AActor *> &other){
    FCollisionQueryParams Params;
    Params.bTraceComplex = false;
    for (int i = 0; i < other.Num(); i++){
        if(AActor *current = other[i]){
            Params.AddIgnoredActor(current);
        }
    }
    return Params;
}




TArray<AActor *> CollisionTracker::copyTracked(){
    return tracked;
}