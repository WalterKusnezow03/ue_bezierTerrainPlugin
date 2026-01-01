#pragma once

#include "CoreMinimal.h"
#include "RoadKeyEnum.h"

class TERRAINPLUGIN_API FRoadKey {
public:
    FRoadKey(){}
    ~FRoadKey(){}

    FRoadKey(int inIndex, ERoadKeyEnum inAxis){
        index = inIndex;
        axis = inAxis;
    }

    FRoadKey(const FRoadKey &other){
        if(this != &other){
            *this = other;
        }
    }

    FRoadKey &operator=(const FRoadKey &other){
        if(this != &other){
            index = other.index;
            axis = other.axis;
        }
        return *this;
    }

    bool operator<(const FRoadKey& other) const
    {
        if (index != other.index)
            return index < other.index;

        return axis < other.axis;
    }

    int index = 0;
    ERoadKeyEnum axis = ERoadKeyEnum::E_xaxis;
};