#include "BoundingBoxSimple.h"


BoundingBoxSimple::BoundingBoxSimple(){
    bottomLeftNear = FVector(0, 0, 0);
    topRightLeft = FVector(0, 0, 0);
}

BoundingBoxSimple::BoundingBoxSimple(TArray<FVector> &vertecies){
    bottomLeftNear = FVector(0, 0, 0);
    topRightLeft = FVector(0, 0, 0);
    updateBoundsIfNeeded(vertecies);
}

BoundingBoxSimple &BoundingBoxSimple::operator=(const BoundingBoxSimple &other){
    if(this != &other){
        bottomLeftNear = other.bottomLeftNear;
        topRightLeft = other.topRightLeft;
    }
    return *this;
}

BoundingBoxSimple::BoundingBoxSimple(const BoundingBoxSimple &other){
    if(this != &other){
        *this = other;
    }
}



BoundingBoxSimple::~BoundingBoxSimple(){

}

void BoundingBoxSimple::updateBoundsIfNeeded(TArray<FVector> &vertecies){
    for (int i = 0; i < vertecies.Num(); i++){
        updateBoundsIfNeeded(vertecies[i]);
    }
}

void BoundingBoxSimple::updateBoundsIfNeeded(FVector &other){
    bottomLeftNear.X = std::min(bottomLeftNear.X, other.X);
    bottomLeftNear.Y = std::min(bottomLeftNear.Y, other.Y);
    bottomLeftNear.Z = std::min(bottomLeftNear.Z, other.Z);

    topRightLeft.X = std::max(topRightLeft.X, other.X);
    topRightLeft.Y = std::max(topRightLeft.Y, other.Y);
    topRightLeft.Z = std::max(topRightLeft.Z, other.Z);
}

bool BoundingBoxSimple::isInsideBoundingbox(FVector &other){
    return other.X > bottomLeftNear.X &&
           other.Y > bottomLeftNear.Y &&
           other.Z > bottomLeftNear.Z &&
           other.X < topRightLeft.X &&
           other.Y < topRightLeft.Y &&
           other.Z < topRightLeft.Z;
}



FVector BoundingBoxSimple::bottomLeftNearVertex(){
    return bottomLeftNear;
}

FVector BoundingBoxSimple::topRightLeftVertex(){
    return topRightLeft;
}



FString BoundingBoxSimple::ToString(){
    FString message = FString::Printf(
        TEXT("BoundingBoxSimple To String: (%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f)"),
        bottomLeftNear.X, bottomLeftNear.Y, bottomLeftNear.Z,
        topRightLeft.X, topRightLeft.Y, topRightLeft.Z
    );
    return message;
}





#include "GameCore/MeshGenBase/MeshData/intersectCache/QuadIntersectFrame.h"
bool BoundingBoxSimple::DoesIntersect(const FVector &origin, const FVector &dir){
    
    //plane intersect
    FVector A0 = bottomLeftNearVertex();
    FVector B2 = topRightLeftVertex();

    FVector connect = B2 - A0;
    float x = connect.X;
    float y = connect.Y;
    float z = connect.Z;

    /*
    stacked:
    y
    1  2
    
    0  3x
    */

    FVector A1 = A0 + FVector(0.0f, y, 0.0f);
    FVector A2 = A0 + FVector(x, y, 0.0f);
    FVector A3 = A0 + FVector(x, 0.0f, 0.0f);

    FVector B0 = A0 + FVector(0.0f, 0.0f, z);
    FVector B1 = A1 + FVector(0.0f, 0.0f, z);
    FVector B3 = A3 + FVector(0.0f, 0.0f, z);

    FVector intersectPoint;
    FQuadIntersectFrame frame;
    //left side
    frame.Setup(A1,B1,B0, A0);
    if(frame.DoesIntersect(origin, dir, intersectPoint)){
        return true;
    }

    //right side
    frame.Setup(A3,B3,B2, A2);
    if(frame.DoesIntersect(origin, dir, intersectPoint)){
        return true;
    }

    //front side
    frame.Setup(A0,B0,B3,A3);
    if(frame.DoesIntersect(origin, dir, intersectPoint)){
        return true;
    }

    //back side
    frame.Setup(A2,B2,B1,A1);
    if(frame.DoesIntersect(origin, dir, intersectPoint)){
        return true;
    }

    //bottom side
    frame.Setup(A1,A0,A3,A2);
    if(frame.DoesIntersect(origin, dir, intersectPoint)){
        return true;
    }

    //top side
    frame.Setup(B0,B1,B2,B3);
    if(frame.DoesIntersect(origin, dir, intersectPoint)){
        return true;
    }

    return false;

}


bool BoundingBoxSimple::InRange(float t, float lower, float higher){
    return t >= lower && t <= higher;
}



#include "DebugPlugin/DebugHelper.h"
void BoundingBoxSimple::Test(){
    TArray<FVector> vertecies = {
        FVector(0, 0, 0),
        FVector(100, 100, 100)
    };

    BoundingBoxSimple instance(vertecies);

    //should be in bound ?
    FVector start(-50,50,50);
    FVector dir(200,0,0);
    FString inBoundResult = instance.DoesIntersect(start, dir) ? TEXT("inBound") : TEXT("outOfBound");

    FString message = FString::Printf(
        TEXT("BoundingBoxSimple::Test %s bound s(%s) dir(%s) RESULT(%s)"), 
        *instance.ToString(),
        *start.ToString(),
        *dir.ToString(),
        *inBoundResult
    );
    DebugHelper::logMessage(message);


    //should be in bound
    start = FVector(50,50,50);
    inBoundResult = instance.DoesIntersect(start, dir) ? TEXT("inBound") : TEXT("outOfBound");

    message = FString::Printf(
        TEXT("BoundingBoxSimple::Test %s in bound s(%s) dir(%s) RESULT(%s)"), 
        *instance.ToString(),
        *start.ToString(),
        *dir.ToString(),
        *inBoundResult
    );
    DebugHelper::logMessage(message);



    //should be in bound
    start = FVector(50,50,200);
    dir = FVector(0,0,-2000);
    inBoundResult = instance.DoesIntersect(start, dir) ? TEXT("inBound") : TEXT("outOfBound");

    message = FString::Printf(
        TEXT("BoundingBoxSimple::Test %s in bound s(%s) dir(%s) RESULT(%s)"), 
        *instance.ToString(),
        *start.ToString(),
        *dir.ToString(),
        *inBoundResult
    );
    DebugHelper::logMessage(message);


    //should be Not in bound
    start = FVector(200,200,200);
    dir = FVector(0,0,-2000);
    inBoundResult = instance.DoesIntersect(start, dir) ? TEXT("inBound") : TEXT("outOfBound");

    message = FString::Printf(
        TEXT("BoundingBoxSimple::Test %s in bound s(%s) dir(%s) RESULT(%s)"), 
        *instance.ToString(),
        *start.ToString(),
        *dir.ToString(),
        *inBoundResult
    );
    DebugHelper::logMessage(message);


    //should be in bound
    start = FVector(-200,-200,-200);
    dir = FVector(400,400,400);
    inBoundResult = instance.DoesIntersect(start, dir) ? TEXT("inBound") : TEXT("outOfBound");

    message = FString::Printf(
        TEXT("BoundingBoxSimple::Test %s in bound s(%s) dir(%s) RESULT(%s)"), 
        *instance.ToString(),
        *start.ToString(),
        *dir.ToString(),
        *inBoundResult
    );
    DebugHelper::logMessage(message);


    //should not be in bound
    start = FVector(-200,-200,-200);
    dir = FVector(400,-400,-400);
    inBoundResult = instance.DoesIntersect(start, dir) ? TEXT("inBound") : TEXT("outOfBound");

    message = FString::Printf(
        TEXT("BoundingBoxSimple::Test %s in bound s(%s) dir(%s) RESULT(%s)"), 
        *instance.ToString(),
        *start.ToString(),
        *dir.ToString(),
        *inBoundResult
    );
    DebugHelper::logMessage(message);
}