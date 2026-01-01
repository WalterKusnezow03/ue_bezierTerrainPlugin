
#pragma once

#include "CoreMinimal.h"

#include "DebugPlugin/DebugHelper.h"

template <typename T>
class TERRAINPLUGIN_API TBSplinePosition
{
public:
	TBSplinePosition(){}
	virtual ~TBSplinePosition(){}

    TBSplinePosition(const TBSplinePosition &other){
        if(this != &other){
            *this = other;
        }
    }

    TBSplinePosition &operator=(const TBSplinePosition &other){
        if(this != &other){
            index = other.index;
            bIsOriginalAnchor = other.bIsOriginalAnchor;
            position = other.position;
        }
        return *this;
    }

    bool IsOriginalAnchor(){
        return bIsOriginalAnchor;
    }

    void SetOriginalAnchorIndex(int indexIn){
        if(indexIn >= 0){
            bIsOriginalAnchor = true;
            index = indexIn;
        }else{
            bIsOriginalAnchor = false;
        }
    }
    
    void SetPosition(T &inPos){
        position = inPos;
    }

    T &GetPosition(){
        return position;
    }

    bool IsSameIndex(int i){
        return i == index;
    }

//protected:
    int index = -1;
    bool bIsOriginalAnchor = false;

    T position;
};


class TERRAINPLUGIN_API FVector2DBSplinePosition : public TBSplinePosition<FVector2D> {

public:
    FVector2DBSplinePosition(){
        bIsOriginalAnchor = false;
    }

    FVector2DBSplinePosition(FVector2D pos){
        position = pos;
        bIsOriginalAnchor = false;
    }

    FVector2DBSplinePosition(FVector2D pos, int index){
        position = pos;
        SetOriginalAnchorIndex(index);
    }


    //marked as true automatically
    FVector2DBSplinePosition(float x, float y, int index){
        position = FVector2D(x, y);
        SetOriginalAnchorIndex(index);
    }

    //marked as false automatically
    FVector2DBSplinePosition(float x, float y){
        position = FVector2D(x, y);
        bIsOriginalAnchor = false;
    }

    // Copy constructor
    FVector2DBSplinePosition(const FVector2DBSplinePosition &other){
        if(this != &other){
            *this = other;
        }
    }

    // Assignment operator
    FVector2DBSplinePosition &operator=(const FVector2DBSplinePosition &other) {
        if(this != &other){
            position = other.position;
            bIsOriginalAnchor = other.bIsOriginalAnchor;
            index = other.index;
        }
        return *this;
    }

    void flip(){
        float copy = position.X;
        position.X = position.Y;
        position.Y = copy;
    }


    void CopyIndexAndAnchorFlag(const FVector2DBSplinePosition &other){
        index = other.index;
        bIsOriginalAnchor = other.bIsOriginalAnchor;
    }
};


class TERRAINPLUGIN_API FVectorBSplinePosition : public TBSplinePosition<FVector> {

public:
    FVectorBSplinePosition(
        const FVector2DBSplinePosition &a, 
        const FVector2DBSplinePosition &b
    ){
        position = FVector(a.position.X, a.position.Y, b.position.Y);
        index = a.index;
        bIsOriginalAnchor = a.bIsOriginalAnchor || b.bIsOriginalAnchor;

        //is dropped.
        if(bIsOriginalAnchor){
            DebugHelper::logMessage("FVectorBSplinePosition::IsOriginalAnchor After Merge Kept Index", index);
        }
    }

    FVectorBSplinePosition(FVector pos, int index){
        position = pos;
        SetOriginalAnchorIndex(index);
    }


private:


};
