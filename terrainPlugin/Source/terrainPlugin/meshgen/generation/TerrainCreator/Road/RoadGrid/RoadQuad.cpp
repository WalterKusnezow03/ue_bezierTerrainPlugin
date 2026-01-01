#include "RoadQuad.h"
#include "DebugPlugin/DebugHelper.h"

RoadQuad::RoadQuad(){

}

RoadQuad::~RoadQuad(){

}

RoadQuad::RoadQuad(const RoadQuad &other){
    if(this != &other){
        *this = other;
    }
}

RoadQuad &RoadQuad::operator=(const RoadQuad &other){
    if(this != &other){
        circle = other.circle;
        innerCircle = other.innerCircle;
        wasModified = other.wasModified;
        center = other.center;
        chunkindexInTerrain = other.chunkindexInTerrain;
    }
    return *this;
}


void RoadQuad::Append(TArray<FVectorBSplinePosition> &vertecies){
    if (vertecies.Num() > 0){
        for(int i = 0; i < vertecies.Num(); i++){
            circle.Add(vertecies[i].GetPosition());
        }
        UpdateCenter();
        wasModified = true;
    }
}

void RoadQuad::Append(TArray<FVector> &vertecies){
    if (vertecies.Num() > 0){
        
        circle.Append(vertecies);
        UpdateCenter();
        wasModified = true;
    }
}

void RoadQuad::UpdateCenter(){
    FVector accumulated;
    int numVertecies = circle.Num();
    if(numVertecies > 0){
        for (int i = 0; i < circle.Num(); i++){
            accumulated += circle[i];
        }
        center = accumulated / numVertecies;
    }
}



void RoadQuad::AppendRoadMesh(MeshData &data, float width){
    /*
    1->2
    |  |
    0<-3
    inner - outer
    */
    GenerateParalellRoad(width);
    AppendRoadMesh(data);
}

void RoadQuad::AppendRoadMesh(MeshData &data){
    if(circle.Num() == innerCircle.Num()){
        for (int i = 1; i < circle.Num(); i++){
            FVector &v0 = innerCircle[i - 1];
            FVector &v1 = innerCircle[i];

            FVector &v2 = circle[i];
            FVector &v3 = circle[i - 1];

            data.appendEfficent(v2, v1, v0, v3); //flipped, is correct.
            //data.appendEfficent(v0, v1, v2, v3); //wrong. expected winding order but wrong.
        }
    }
}


void RoadQuad::GenerateParalellRoad(
    TArray<FVector> &outerOut,
    TArray<FVector> &innerOut,
    float width
){
    if(!wasModified){
        if(circle.Num() == innerCircle.Num() && circle.Num() != 0){
            outerOut = circle;
            innerOut = innerCircle;   
        }else{
            wasModified = true;
        }
    }


    //DebugHelper::logMessage("RoadQuad::Generate Road from A", circle.Num());
    GenerateParalellRoad(width); //regenerate
    outerOut = circle;
    innerOut = innerCircle;
}

void RoadQuad::GenerateParalellRoad(float width){
    if(!wasModified){
        return;
    }
    if(circle.Num() == 0){
        return;
    }
    //DebugHelper::logMessage("RoadQuad::Generate Road from B", circle.Num());

    wasModified = false;

    innerCircle.SetNum(circle.Num());
    FVector normal;
    for (int i = 0; i < circle.Num(); i++){
        FVector &current = circle[i];
        normal = (center - current);// AB = B - A
        normal.Z = 0.0f;
        normal = normal.GetSafeNormal(); 

        innerCircle[i] = current + normal * width; //update
    }
}

TArray<FVector> &RoadQuad::GetCirlce(){
    return circle;
}

TArray<FVector> &RoadQuad::GetInnerCircle(){
    return innerCircle;
}

FVector &RoadQuad::GetCenter(){
    return center;
}


void RoadQuad::RemoveOffset(FVector &offset){
    center -= offset;
    for (int i = 0; i < circle.Num(); i++){
        circle[i] -= offset;
    }
    for (int i = 0; i < innerCircle.Num(); i++){
        innerCircle[i] -= offset;
    }
}