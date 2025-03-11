#pragma once

#include "CoreMinimal.h"
#include "p2/meshgen/ELod.h"
#include <map>

class P2_API LodCheckContainer{

public:
    LodCheckContainer();
    LodCheckContainer(FVector &a, FVector &b);
    ~LodCheckContainer();

    void checkLod(FVector &a, FVector &b);

    void modifyUpperDistanceLimitFor(ELod lod, int newMaxDistance);
    bool lodWasEdgeCaseToELodFar();
    bool lodWasEdgeCaseToELodMiddle();
    bool hideActorByLod();
    ELod lod();

private:
    std::map<ELod, int> distances;

    bool hideActorFlag = false;

    int edgeDistanceFrom = 1000;

    FVector thisLocation;
    FVector playerLocation;

    ELod lodFound = ELod::lodFar;
    bool wasLodFarEdgeCase = false;
    bool wasLodMiddleEdgeCase = false;

    void checkLod();
    ELod lodLevelByDistanceTo(bool &isEdgeCase);
    bool isInRange(FVector &a, int maxDistance, int &smallestEdge);
    int maxDistanceForLod(ELod lodLevel);
};